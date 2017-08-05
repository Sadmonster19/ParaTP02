#include "RatHunter.h"

RatHunter::RatHunter(unsigned int positionX_, unsigned int positionY_)
    : Character(positionX_, positionY_)
{
}

void RatHunter::initPossibleMovement() {
	possibleMovement.emplace_back(Movement::getPositionForDirection(Direction::NORTH));
	possibleMovement.emplace_back(Movement::getPositionForDirection(Direction::EAST));
	possibleMovement.emplace_back(Movement::getPositionForDirection(Direction::SOUTH));
	possibleMovement.emplace_back(Movement::getPositionForDirection(Direction::WEST));
}

bool RatHunter::move(Position pos)
{
    //TODO: ASK MAP TO MOVE THERE
    return false;
}

void RatHunter::findBestPath(Position pos)
{

}