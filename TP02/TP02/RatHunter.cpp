#include "RatHunter.h"

RatHunter::RatHunter(Position p_)
	: Character(p_)
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