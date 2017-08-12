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
    Position wanted = findNextMovement(getMapObjectPositions(RAT, id)); //Normally would find the best path

    unsigned int movement[2] = { wanted.x, wanted.y };
    MPI_Send(&movement, _countof(movement), MPI_INT, 0, id, MPI_COMM_WORLD);

    return wanted;
}

vector<Position> RatHunter::findAllRatsInZone(vector<Position> rats) {
	return AStar::findAllRatsInZone(*this, rats);
}