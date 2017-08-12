#include "Rat.h"

Rat::Rat(Position p_)
    : Character(p_)
{
	initPossibleMovement();
}

void Rat::initPossibleMovement() {
	possibleMovement.emplace_back(Position::getPositionForDirection(Direction::NORTH_WEST));
	possibleMovement.emplace_back(Position::getPositionForDirection(Direction::NORTH));
	possibleMovement.emplace_back(Position::getPositionForDirection(Direction::NORTH_EAST));
	possibleMovement.emplace_back(Position::getPositionForDirection(Direction::EAST));
	possibleMovement.emplace_back(Position::getPositionForDirection(Direction::SOUTH_EAST));
	possibleMovement.emplace_back(Position::getPositionForDirection(Direction::SOUTH));
	possibleMovement.emplace_back(Position::getPositionForDirection(Direction::SOUTH_WEST));
	possibleMovement.emplace_back(Position::getPositionForDirection(Direction::WEST));
}

Position Rat::move(int id)
{
    Position wanted;
    
    if (isNearRatHunter(getRatsInPanic(id)))
        panicLevel = 5;

    if (panicLevel > 0)
        wanted = findNextMovement(getMapObjectPositions(DOOR, id));
    else
        wanted = findNextMovement(getMapObjectPositions(CHEESE, id));

    int movement[2] = { wanted.x, wanted.y };
//    MPI_Send(&movement, _countof(movement), MPI_INT, 0, id, MPI_COMM_WORLD);

    return wanted;
}

bool Rat::isNearRatHunter(vector<Position> rats) {
    return std::find(rats.begin(), rats.end(), position) != rats.end();
}

vector<Position> Rat::getRatsInPanic(int id) {
    vector<Position> ratsInPanic;

    int arrayDim;
    MPI_Recv(&arrayDim, 1, MPI_INT, id, id, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    int* positions = new (int[arrayDim]);
    MPI_Recv(positions, arrayDim, MPI_INT, id, id, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    for (int i = 0; i < arrayDim; i += 2) {
        ratsInPanic.push_back(Position{ positions[i], positions[i + 1] });
    }

    return ratsInPanic;
}