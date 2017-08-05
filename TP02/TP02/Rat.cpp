#include "Rat.h"

Rat::Rat(unsigned int positionX_, unsigned int positionY_)
    : Character(positionX_, positionY_)
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

void Rat::findBestPath(Position pos)
{

}
