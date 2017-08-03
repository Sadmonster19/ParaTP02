#ifndef MAPELEMENT_H
#define MAPELEMENT_H

#include <iostream>
#include <vector>

using namespace std;

struct Position
{
    unsigned int x;
    unsigned int y;

	Position(unsigned int _x, unsigned int _y) : x{ _x }, y{ _y } {}
};

class Character
{
public:
    Character() = default;
    Character(unsigned int positionX_, unsigned int positionY_);
    ~Character() = default;

protected:
	Position position;
	vector<Position> bestPath;

	virtual bool move(Position) = 0;
	virtual void findBestPath(Position) = 0;

public:
    
};

#endif //MAPELEMENT_H
