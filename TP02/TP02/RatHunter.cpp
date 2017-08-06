#include "RatHunter.h"

RatHunter::RatHunter(Position p_)
	: Character(p_)
{
}

void RatHunter::initPossibleMovement() {
	possibleMovement.emplace_back(Position::getPositionForDirection(Direction::NORTH));
	possibleMovement.emplace_back(Position::getPositionForDirection(Direction::EAST));
	possibleMovement.emplace_back(Position::getPositionForDirection(Direction::SOUTH));
	possibleMovement.emplace_back(Position::getPositionForDirection(Direction::WEST));
}

bool RatHunter::move(Position pos)
{
    //TODO: ASK MAP TO MOVE THERE
    return false;
}