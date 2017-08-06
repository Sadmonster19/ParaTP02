#include "Character.h"
#include "AStar.h"

Character::Character(Position p_)
    : position{ p_ }
{
}

Position Character::getPosition() {
	return position;
}

vector<Position> Character::getPossibleMovement() {
	return possibleMovement;
}

vector<Position> Character::getBestPath() {
	return bestPath;
}

void Character::findBestPath(MapObject mo) {
	bestPath = AStar::findBestPath(initialMap, this, mo);
}
