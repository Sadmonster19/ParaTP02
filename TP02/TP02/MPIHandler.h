#ifndef MPIHANDLER_H
#define MPIHANDLER_H

#include "WorldMap.h"
#include <mpi.h>

class MPIHandler
{
public:
    MPIHandler() = default;
    ~MPIHandler();

    MPIHandler(int& argc, char* argv[]);

    //Can't be copied
    MPIHandler(const MPIHandler&) = delete;
    MPIHandler& operator=(const MPIHandler&) = delete;
    
    //Can't be moved
    MPIHandler(MPIHandler&&) = delete;
    MPIHandler& operator=(MPIHandler&&) = delete;

private:
    MPI_Group worldGroupId;
    MPI_Comm mapCommId;
    MPI_Group mapGroupId;
    MPI_Comm ratCommId;
    MPI_Group ratGroupId;
    MPI_Comm ratHunterCommId;
    MPI_Group ratHunterGroupId;
    int rank, processCount, provided, ratCount, ratHunterCount;

public:
    int getRank();
    int getSize() const;
};

#endif //MPIHANDLER_H