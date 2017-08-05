#include "Character.h"

Character::Character(unsigned int positionX_, unsigned int positionY_)
    : position{ positionX_, positionY_ }
{
}

Position Character::getPosition() {
	return position;
}

vector<Position> Character::getPossibleMovement() {
	return possibleMovement;
}

