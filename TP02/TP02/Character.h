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
	bool isOutOfBounds(Position p);

public:
    void displayMap();
    void setInitialMap(int id);
    MapStructure getInitialMap();
	Position getPosition();
	void setPosition(Position);

	Position findNextMovement(vector<Position>);
	virtual bool canWalkOn(Position);
	vector<Position> getPossibleMovement();
    vector<Position> getMapObjectPositions(MapObject object, int id);
};

#endif //CHARACTER_H
