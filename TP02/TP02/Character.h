#ifndef CHARACTER_H
#define CHARACTER_H

#include "Movement.h"

#include <vector>

using namespace std;

class Character
{
public:
    Character() = default;
    Character(unsigned int positionX_, unsigned int positionY_);
    ~Character() = default;

protected:
	Position position;
	vector<Position> possibleMovement;
	vector<Position> bestPath;

	virtual void initPossibleMovement() = 0;
	virtual bool move(Position) = 0;
	virtual void findBestPath(Position) = 0;

public:
	Position getPosition();
	vector<Position> getPossibleMovement();
};

#endif //CHARACTER_H
