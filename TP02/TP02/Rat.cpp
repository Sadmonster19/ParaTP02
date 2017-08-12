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
    if (panicLevel > 0)
        wanted = findNextMovement(getMapObjectPositions(CHEESE, id)); //Normally would find the best path
    else
        wanted = findNextMovement(getMapObjectPositions(CHEESE, id)); //Normally would find the best path

    unsigned int movement[2] = { wanted.x, wanted.y };
    MPI_Send(&movement, _countof(movement), MPI_INT, 0, id, MPI_COMM_WORLD);

    return wanted;
}

bool Rat::isNearRatHunter(vector<Position> rats) {
    return std::find(rats.begin(), rats.end(), position) != rats.end();
}