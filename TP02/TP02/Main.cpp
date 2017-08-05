#include "mpi.h"
#include "MPIHandler.h"
#include "WorldMap.h"
#include <iostream>
#include <thread>

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
		city.endGame();

    }
    else if (e.getRank() <= e.getRatCount()){
        Rat* ratCharacter;
        int infos[2];
		bool gameOver = false;
		bool isAlive = true;
        MPI_Recv(&infos, _countof(infos), MPI_2INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        ratCharacter = &Rat{(unsigned int)infos[0], (unsigned int)infos[1]};
		Position pos = ratCharacter->getPosition();

		cout << "Process " << e.getRank() << ", IM PICKEL RAT and my position is (" << pos.x << ", " << pos.y << ")" << endl;

		int res[1];
		MPI_Recv(&res, _countof(res), MPI_2INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

		while (!gameOver && isAlive) {
			unsigned int movement[2] = {pos.x+1, pos.y+1};
			MPI_Send(&movement, _countof(movement), MPI_2INT, 0, 0, MPI_COMM_WORLD);
			int result[3];
			MPI_Recv(&result, _countof(result), MPI_2INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
			if (result[0]) {
				ratCharacter->setPosition(Position{ pos.x + 1, pos.y + 1 });
				cout << "Process " << e.getRank() << ", new position (" << ratCharacter->getPosition().x << ", " << ratCharacter->getPosition().y << ")" << endl;		
			}
			if (result[1]) {
				gameOver = true;
				cout << "Process " << e.getRank() << ", GAMEOVER" << endl;
			}
			if (!result[2]) {
				isAlive = false;
				cout << "Process " << e.getRank() << ", DEAD" << endl;
			}
		}
    }
    else if(e.getRank() > e.getRatCount()){
        RatHunter* ratHunterCharacter;
        int res[3];
        MPI_Recv(&res, _countof(res), MPI_2INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        if (res[0] == ratHunter) {
            ratHunterCharacter = &RatHunter{ (unsigned int)res[1], (unsigned int)res[2] };
        }
		Position pos = ratHunterCharacter->getPosition();
		cout << "Process " << e.getRank() << ", I am a Cat and my position is (" << pos.x << ", " << pos.y << ")" << endl;
    }

    return 0;
}