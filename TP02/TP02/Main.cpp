#include "mpi.h"
#include "EventHandler.h"
#include <iostream>
#include <thread>
#include <chrono>

//argv : 0 == TP02.exe, 1 == rat, 2 == ratHunter, 3 == file name of map 
//0 == map, 1-ratcount == ratHunter, racount-ratcount+huntercount == rat 
//cmd in debug : "mpiexec -n x TP02.exe y z fileName"
//x = ratCount+ratHunterCount + 1,  y = ratCount z = ratHunterCount
//Ex : "mpiexec -n 8 TP02.exe 3 4 Map1.txt"

int main(int argc, char* argv[]) {
           
    int r, p;

    EventHandler e{};
    e.initHandler(argc, argv, r, p);

    if (r == 0) {
        WorldMap city{ argv[3] };

        while (!city.isGameDone()) {
            city.displayMap();
            int done;
            MPI_Recv(&done, 1, MPI_2INT, 1, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            city.endGame(done);
        }
    }
    else if (r == 1) {
        int bob = 0;
        int done = 1;
        while(bob < 5) {
            cout << r << 1 << endl;
            done = 0;
            MPI_Send(&done, 1, MPI_2INT, 0, 0, MPI_COMM_WORLD);
            bob++;
        }
    }

    if (r == 1) {
        int done = 0;
        if (done == 0) {
            cout << r << 2 << endl;
            done = 1;
            MPI_Send(&done, 1, MPI_2INT, 0, 0, MPI_COMM_WORLD);
        }
    }


    /*else if (r == 1) {
        cout << 1 << endl;
        //char helloStr[12];
        //MPI_Recv(helloStr, _countof(helloStr), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        //printf(helloStr);
    }
    else if (r == 2)
        cout << 2 << endl;*/

    MPI_Finalize();

    return 0;
}