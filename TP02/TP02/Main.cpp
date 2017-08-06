#include <iostream>

#include "mpi.h"
#include "MPIHandler.h"
#include "WorldMap.h"
#include "Rat.h"
#include "RatHunter.h"
#include "AStar.h"

//argv : 0 == TP02.exe, 1 == rat, 2 == ratHunter, 3 == file name of map 
//0 == map, 1-ratcount == ratHunter, racount-ratcount+huntercount == rat 
//cmd in debug : "mpiexec -n x TP02.exe y z fileName"
//x = ratCount+ratHunterCount + 1,  y = ratCount z = ratHunterCount
//Ex : "mpiexec -n 6 TP02.exe 3 2 Map1.txt"

int main(int argc, char* argv[]) {

    /*WorldMap wp{ "Maps/Map1.txt" };
    
    wp.sendInitialMapToCharacter(1);
    wp.displayMap();*/
    
    MPIHandler e{ argc, argv };

    if (e.getRank() == 0) {
        WorldMap city{ argv[3] };

		city.displayMap();
        city.initCharacters();

		city.gameReady = true;
		_sleep(10);
		city.endGame();

    }
    else if (e.getRank() <= e.getRatCount()){
		bool gameOver = false;
		bool isAlive = true;

		//Get info to initialise rat
		int infos[2];
        MPI_Recv(&infos, _countof(infos), MPI_2INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        Rat rat{Position((unsigned int)infos[0], (unsigned int)infos[1])};
        
        cout << e.getRank() << endl;
        rat.setInitialMap();
        rat.displayMap();
        //rat.findBestPath(CHEESE);

		cout << "Process " << e.getRank() << ", IM PICKEL RAT and my position is (" << rat.getX() << ", " << rat.getY() << ")" << endl;

		//Wait until you receive that the game has started
		int res;
        MPI_Recv(&res, 1, MPI_2INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

		while (!gameOver && isAlive) {
			Position wanted = Position(rat.getX(), rat.getY() + 1); //Normally would find the best path

			unsigned int movement[2] = {wanted.x, wanted.y};
			MPI_Send(&movement, _countof(movement), MPI_2INT, 0, 0, MPI_COMM_WORLD);

			int result[3];	//Succes, gameDone, isAlive
			MPI_Recv(&result, _countof(result), MPI_2INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

			if (result[0]) {	//Move success
				rat.setPosition(wanted);
				cout << "Process " << e.getRank() << ", new position (" << rat.getPosition().x << ", " << rat.getPosition().y << ")" << endl;		
			}
			if (result[1]) {	//Game is over
				gameOver = true;
				cout << "Process " << e.getRank() << ", GAMEOVER" << endl;
			}
			if (!result[2]) {	//Rat is dead!
				isAlive = false;
				cout << "Process " << e.getRank() << ", IS DEAD" << endl;
			}
		}
    }
    else if(e.getRank() > e.getRatCount()){
        RatHunter* ratHunterCharacter;
        int res[3];
        MPI_Recv(&res, _countof(res), MPI_2INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        if (res[0] == MapObject::HUNTER) {
            ratHunterCharacter = &RatHunter{ Position{ res[1], res[2] } };
        }
		Position pos = ratHunterCharacter->getPosition();
		cout << "Process " << e.getRank() << ", I am a Cat and my position is (" << pos.x << ", " << pos.y << ")" << endl;
    }

    return 0;
}