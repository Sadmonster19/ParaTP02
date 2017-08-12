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
        MPI_Recv(&infos, _countof(infos), MPI_INT, 0, e.getRank(), MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        Rat rat{Position((unsigned int)infos[0], (unsigned int)infos[1])};
        
        rat.setInitialMap(e.getRank());

		cout << "Process " << e.getRank() << ", IM PICKEL RAT and my position is (" << rat.getX() << ", " << rat.getY() << ")" << endl;

		//Wait until you receive that the game has started
		int res;
        MPI_Recv(&res, 1, MPI_INT, 0, e.getRank(), MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

		while (!gameOver && isAlive) {
            Position wanted = rat.findNextMovement(rat.getMapObjectPositions(CHEESE, e.getRank())); //Normally would find the best path

			unsigned int movement[2] = {wanted.x, wanted.y};
			MPI_Send(&movement, _countof(movement), MPI_INT, 0, e.getRank(), MPI_COMM_WORLD);

			int result[3];	//Succes, gameDone, isAlive
			MPI_Recv(&result, _countof(result), MPI_INT, 0, e.getRank(), MPI_COMM_WORLD, MPI_STATUSES_IGNORE);   

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
        bool gameOver = false;
        bool isAlive = true;

        //Get info to initialise rat
        int infos[2];
        MPI_Recv(&infos, _countof(infos), MPI_INT, 0, e.getRank(), MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        RatHunter hunter{ Position((unsigned int)infos[0], (unsigned int)infos[1]) };

        hunter.setInitialMap(e.getRank());

        cout << "Process " << e.getRank() << ", IM a RATHUNTER and my position is (" << hunter.getX() << ", " << hunter.getY() << ")" << endl;

        //Wait until you receive that the game has started
        int res;
        MPI_Recv(&res, 1, MPI_INT, 0, e.getRank(), MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

        while (!gameOver && isAlive) {
            Position wanted = hunter.findNextMovement(hunter.getMapObjectPositions(RAT, e.getRank())); //Normally would find the best path

            unsigned int movement[2] = { wanted.x, wanted.y };
            MPI_Send(&movement, _countof(movement), MPI_INT, 0, e.getRank(), MPI_COMM_WORLD);

            int result[3];	//Succes, gameDone, isAlive
            MPI_Recv(&result, _countof(result), MPI_INT, 0, e.getRank(), MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

            if (result[0]) {	//Move success
                hunter.setPosition(wanted);
                cout << "Process " << e.getRank() << ", new position (" << hunter.getPosition().x << ", " << hunter.getPosition().y << ")" << endl;
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

    return 0;
}