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
	void findBestPath(MapStructure, MapObject);

public:
	Position getPosition();
	vector<Position> getPossibleMovement();
};

#endif //CHARACTER_H
