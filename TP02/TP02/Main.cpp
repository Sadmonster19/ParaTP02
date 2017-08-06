#include <iostream>
#include <thread>
#include <chrono>

#include "mpi.h"
#include "MPIHandler.h"
#include "WorldMap.h"
#include "Rat.h"
#include "RatHunter.h"

//argv : 0 == TP02.exe, 1 == rat, 2 == ratHunter, 3 == file name of map 
//0 == map, 1-ratcount == ratHunter, racount-ratcount+huntercount == rat 
//cmd in debug : "mpiexec -n x TP02.exe y z fileName"
//x = ratCount+ratHunterCount + 1,  y = ratCount z = ratHunterCount
//Ex : "mpiexec -n 6 TP02.exe 3 2 Map1.txt"

int main(int argc, char* argv[]) {

    MPIHandler e{ argc, argv };

    if (e.getRank() == 0) {
        WorldMap city{ argv[3] };

        vector<Position> p = city.getRatHuntersPosition();

        for (auto pos : p) {
            cout << pos.x << endl;
        }

        city.initCharacters();
    }
    else if (e.getRank() <= e.getRatCount()){
        Rat* ratCharacter;
        int res[3];
        MPI_Recv(&res, _countof(res), MPI_2INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        if (res[0] == MapObject::RAT) {
			ratCharacter = &Rat{ Position {res[1], res[2] } };
        }

        cout << e.getRank() << "Position en x :" << ratCharacter->getPosition().x << endl;
    }
    else if(e.getRank() <= e.getRatCount() + e.getRatHunterCount()){
        RatHunter* ratHunterCharacter;
        int res[3];
        MPI_Recv(&res, _countof(res), MPI_2INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        if (res[0] == MapObject::HUNTER) {
            ratHunterCharacter = &RatHunter{ Position{ res[1], res[2] } };
        }

        cout << e.getRank() << "Position en x :" << ratHunterCharacter->getPosition().x << endl;
    }
    
    /*if (e.getRank == 0) {

        while (!city.isGameDone()) {
        city.displayMap();
        int done;
        MPI_Recv(&done, 1, MPI_2INT, 1, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        city.endGame(done);
        }
    }
    else if (e.getRank == 1) {
        int bob = 0;
        int done = 1;
        while(bob < 5) {
            cout << e.getRank << 1 << endl;
            done = 0;
            MPI_Send(&done, 1, MPI_2INT, 0, 0, MPI_COMM_WORLD);
            bob++;
        }
    }

    if (e.getRank == 1) {
        int done = 0;
        if (done == 0) {
            cout << e.getRank << 2 << endl;
            done = 1;
            MPI_Send(&done, 1, MPI_2INT, 0, 0, MPI_COMM_WORLD);
        }
    }*/


    /*else if (r == 1) {
        cout << 1 << endl;
        //char helloStr[12];
        //MPI_Recv(helloStr, _countof(helloStr), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        //printf(helloStr);
    }
    else if (r == 2)
        cout << 2 << endl;*/


    return 0;
}