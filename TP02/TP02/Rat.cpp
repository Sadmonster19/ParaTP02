#include "Rat.h"

Rat::Rat(unsigned int positionX_, unsigned int positionY_)
    : Character(positionX_, positionY_)
{
}

bool Rat::move(Position pos)
{
	//TODO: ASK MAP TO MOVE THERE
	return false;
}

void Rat::findBestPath(Position pos)
{

}

Position Rat::findClosestCheese()
{
	return Position(0, 0);
}
