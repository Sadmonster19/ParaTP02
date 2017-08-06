#include "Character.h"
#include "AStar.h"

Character::Character(Position p_)
    : position{ p_ }
{
}

Position Character::getPosition() {
	return position;
}

void Character::setPosition(Position p) {
	position = p;
}

vector<Position> Character::getPossibleMovement() {
	return possibleMovement;
}

vector<Position> Character::getBestPath() {
	return bestPath;
}

MapStructure Character::getInitialMap() {
	return initialMap;
}

MapObject Character::getMapObjectForPosition(Position p) {
	return initialMap[p.y][p.x];
}

void Character::findBestPath(MapObject mo) {
	bestPath = AStar::findBestPath(this, mo);
}