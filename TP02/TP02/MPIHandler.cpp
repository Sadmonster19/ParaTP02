#include "MPIHandler.h"

MPIHandler::MPIHandler(int& argc, char* argv[]) {
    ratCount = atoi(argv[1]);
    ratHunterCount = atoi(argv[2]);
    
    int rc = MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    if (rc != MPI_SUCCESS) {
        printf("Error on MPI init. \n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }
    
    // Get the number of processes.
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);

    // Get the individual process ID.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

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
}

MPIHandler::~MPIHandler() {
    MPI_Finalize();
}

int MPIHandler::getRank() const {
    return rank;
}

int MPIHandler::getSize() const {
    return processCount;
}