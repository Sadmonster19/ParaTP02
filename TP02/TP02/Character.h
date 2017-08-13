#ifndef CHARACTER_H
#define CHARACTER_H

#include "Tools.h"
#include "MPIHandler.h"
#include <mutex>

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
    MapStructure initialMap;

	virtual void initPossibleMovement() = 0;

public:
    void displayMap();
    void setInitialMap(int id);
    MapStructure getInitialMap();
	Position getPosition();
	void setPosition(Position);
	unsigned int getX() {
		return position.x;
	}
	unsigned int getY() {
		return position.y;
	}

	Position findNextMovement(vector<Position>);
	virtual bool canWalkOn(Position);
	vector<Position> getPossibleMovement();
    vector<Position> getMapObjectPositions(MapObject object, int id);
};

#endif //CHARACTER_H
