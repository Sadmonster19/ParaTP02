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
	vector<Position> bestPath;
    MapStructure initialMap;

	virtual void initPossibleMovement() = 0;
	virtual bool move(Position) = 0;

public:
    void displayMap();
    void setInitialMap();
    MapStructure getInitialMap();
	Position getPosition();
	void setPosition(Position);
	unsigned int getX() {
		return position.x;
	}
	unsigned int getY() {
		return position.y;
	}

	void findBestPath(MapObject);
	MapObject getMapObjectForPosition(Position);
	vector<Position> getPossibleMovement();
	vector<Position> getBestPath();
    void popLastPosition();

};

#endif //CHARACTER_H
