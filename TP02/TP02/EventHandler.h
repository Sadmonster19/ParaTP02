#include "mpi.h"
#include "WolrdMap.h"

class EventHandler
{
public:
    EventHandler() = default;
    ~EventHandler() = default;

private:
    MPI_Group worldGroupId;

    MPI_Comm mapCommId;
    MPI_Group mapGroupId;

    MPI_Comm ratCommId;
    MPI_Group ratGroupId;

    MPI_Comm ratHunterCommId;
    MPI_Group ratHunterGroupId;

    int provided, ratCount, ratHunterCount;

public:
    void initHandler(int& argc, char* argv[], int& p, int& r);
};