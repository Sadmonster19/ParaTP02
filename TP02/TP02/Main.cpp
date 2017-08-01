#include "mpi.h"
#include <iostream>


int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        char helloStr[] = "Hello world";
        MPI_Send(helloStr, _countof(helloStr), MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    }
    else if (rank == 1) {
        char helloStr[12];
        MPI_Recv(helloStr, _countof(helloStr), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        printf("Rank 1 received string %s from Rank 0\n", helloStr);
    }

    MPI_Finalize();
    return 0;
}