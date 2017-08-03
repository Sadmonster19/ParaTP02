#include "RatHunter.h"

RatHunter::RatHunter(unsigned int positionX_, unsigned int positionY_)
    : Character(positionX_, positionY_)
{
}

bool RatHunter::move(Position pos)
{
    //TODO: ASK MAP TO MOVE THERE
    return false;
}

void RatHunter::findBestPath(Position pos)
{

}

Position RatHunter::findClosestCheese()
{
    return Position(0, 0);
}