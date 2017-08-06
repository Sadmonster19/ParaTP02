#ifndef CHARACTER_H
#define CHARACTER_H

#include "Tools.h"

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
    //void getInitialMap();
	Position getPosition();
	unsigned int getX() {
		return position.x;
	}
	unsigned int getY() {
		return position.y;
	}
	void setPosition(Position pos) {
		position = pos;
	}
	void findBestPath(MapObject);
	vector<Position> getPossibleMovement();
	vector<Position> getBestPath();

};

#endif //CHARACTER_H
