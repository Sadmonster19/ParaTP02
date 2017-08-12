#include "RatHunter.h"
#include "AStar.h"

RatHunter::RatHunter(Position p_)
	: Character(p_)
{
	initPossibleMovement();
}

void RatHunter::initPossibleMovement() {
	possibleMovement.emplace_back(Position::getPositionForDirection(Direction::NORTH));
	possibleMovement.emplace_back(Position::getPositionForDirection(Direction::EAST));
	possibleMovement.emplace_back(Position::getPositionForDirection(Direction::SOUTH));
	possibleMovement.emplace_back(Position::getPositionForDirection(Direction::WEST));
}

Position RatHunter::move(int id)
{
    vector<Position> ratsPositions = getMapObjectPositions(RAT, id);

    sendSreams(ratsPositions, id);

    Position wanted = findNextMovement(ratsPositions);

    int movement[2] = { wanted.x, wanted.y };
//    MPI_Send(&movement, _countof(movement), MPI_INT, 0, id, MPI_COMM_WORLD);
    int movement[2] = { wanted.x, wanted.y };
    MPI_Send(&movement, _countof(movement), MPI_INT, 0, id, MPI_COMM_WORLD);

    return wanted;
}

vector<Position> RatHunter::findAllRatsInZone(vector<Position> rats) {
	return AStar::findAllRatsInZone(*this, rats);
}

void RatHunter::sendSreams(vector<Position> ratsPositions, int id) {
    int scream = 0;
    vector<Position> ratsInZone = findAllRatsInZone(ratsPositions);

    if (ratsInZone.empty()) {
        MPI_Send(&scream, 1, MPI_INT, 0, id, MPI_COMM_WORLD);
    }
    else {
        scream = 1;
        MPI_Send(&scream, 1, MPI_INT, 0, id, MPI_COMM_WORLD);

        int arrayDim = ratsInZone.size() * 2;
        int* positions = new (int[arrayDim]);

        MPI_Send(&arrayDim, 1, MPI_INT, 0, id, MPI_COMM_WORLD);

        int i = 0;
        for (auto position : ratsInZone) {
            positions[i] = position.x;
            positions[i + 1] = position.y;
            i += 2;
        }
        MPI_Send(positions, arrayDim, MPI_INT, 0, id, MPI_COMM_WORLD);
    }
}