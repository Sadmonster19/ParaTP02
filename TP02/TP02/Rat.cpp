#include "Rat.h"

Rat::Rat(Position p_)
    : Character(p_)
{
}

void Rat::initPossibleMovement() {
	possibleMovement.emplace_back(Movement::getPositionForDirection(Direction::NORTH_WEST));
	possibleMovement.emplace_back(Movement::getPositionForDirection(Direction::NORTH));
	possibleMovement.emplace_back(Movement::getPositionForDirection(Direction::NORTH_EAST));
	possibleMovement.emplace_back(Movement::getPositionForDirection(Direction::EAST));
	possibleMovement.emplace_back(Movement::getPositionForDirection(Direction::SOUTH_EAST));
	possibleMovement.emplace_back(Movement::getPositionForDirection(Direction::SOUTH));
	possibleMovement.emplace_back(Movement::getPositionForDirection(Direction::SOUTH_WEST));
	possibleMovement.emplace_back(Movement::getPositionForDirection(Direction::WEST));
}

bool Rat::move(Position pos)
{
	//TODO: ASK MAP TO MOVE THERE
	return false;
}
