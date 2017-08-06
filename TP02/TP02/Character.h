#ifndef CHARACTER_H
#define CHARACTER_H

#include "Movement.h"
#include "WorldMap.h"

#include <vector>

using namespace std;

class Character
{
public:
    Character() = default;
    Character(Position);
    ~Character() = default;

protected:
	Position position;
	vector<Position> possibleMovement;
	vector<Position> bestPath;

	virtual void initPossibleMovement() = 0;
	virtual bool move(Position) = 0;

public:
	void findBestPath(MapStructure, MapObject);
	Position getPosition();
	vector<Position> getPossibleMovement();
	vector<Position> getBestPath();
};

#endif //CHARACTER_H
