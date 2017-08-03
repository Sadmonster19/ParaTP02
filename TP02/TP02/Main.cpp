#include "mpi.h"
#include "WolrdMap.h"
#include <iostream>
#include <thread>
#include <chrono>

//argv : 0 == TP02.exe, 1 == rat, 2 == ratHunter, 3 == file name of map 
//0 == map, 1-ratcount == ratHunter, racount-ratcount+huntercount == rat 
//cmd in debug : "mpiexec -n x TP02.exe y z fileName"
//x = ratCount+ratHunterCount + 1,  y = ratCount z = ratHunterCount
//Ex : "mpiexec -n 8 TP02.exe 3 4 Map1.txt"

int main(int argc, char* argv[]) {
    MPI_Group worldGroupId;

    MPI_Comm mapCommId;
    MPI_Group mapGroupId;

    MPI_Comm ratCommId;
    MPI_Group ratGroupId;

    MPI_Comm ratHunterCommId;
    MPI_Group ratHunterGroupId;

    int p, r, provided;
    int ratCount = atoi(argv[1]);
    int ratHunterCount = atoi(argv[2]);

    int rc = MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    if (rc != MPI_SUCCESS) {
        printf("Error on MPI init. \n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    // Get the number of processes.
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    // Get the individual process ID.
    MPI_Comm_rank(MPI_COMM_WORLD, &r);

    // Get a group identifier for MPI_COMM_WORLD.
    MPI_Comm_group(MPI_COMM_WORLD, &worldGroupId);
    
    // Create group for map
    int* mapRanks = new int[1];
    mapRanks[0] = 0;
    MPI_Group_incl(worldGroupId, 1, mapRanks, &mapGroupId);
    MPI_Comm_create(MPI_COMM_WORLD, mapGroupId, &mapCommId);

    //List rat processes and create their group
    int* ratRanks = new int[ratCount];
    int j = 0;
    for (int i = 1; i <= ratCount; ++i) {
        ratRanks[j] = i;
        j++;
    }
    MPI_Group_incl(worldGroupId, ratCount, ratRanks, &ratGroupId);
    MPI_Comm_create(MPI_COMM_WORLD, ratGroupId, &ratCommId);

    //List ratHunter processes and create their group
    int* ratHunterRanks = new int[ratHunterCount];
    j = 0;
    for (int i = ratCount + 1; i <= ratCount + ratHunterCount; ++i) {
        ratHunterRanks[j] = i;
        j++;
    }
    MPI_Group_incl(worldGroupId, ratHunterCount, ratHunterRanks, &ratHunterGroupId);
    MPI_Comm_create(MPI_COMM_WORLD, ratHunterGroupId, &ratHunterCommId);

    if (r == 0) {
        WorldMap city{ argv[3] };

        while (!city.isGameDone()) {
            city.displayMap();
            int done;
            MPI_Recv(&done, 1, MPI_2INT, 1, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            city.endGame(done);
        }
        // city.displayMap();
        
        //char[] helloStr = "Hello World";
        //MPI_Send(helloStr, _countof(helloStr), MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    }
    else if (r == 1) {
        int bob = 0;
        int done = 1;
        while(bob < 5) {
            //std::this_thread::sleep_for(std::chrono::seconds(5));
            cout << r << 1 << endl;
            done = 0;
            MPI_Send(&done, 1, MPI_2INT, 0, 0, MPI_COMM_WORLD);
            bob++;
        }
    }

    if (r == 1) {
        int done = 0;
        if (done == 0) {
            //std::this_thread::sleep_for(std::chrono::seconds(5));
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