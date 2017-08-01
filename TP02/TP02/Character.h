#ifndef MAPELEMENT_H
#define MAPELEMENT_H

#include <iostream>
#include <vector>

using namespace std;

struct Position
{
	int x;
	int y;

	Position(int _x, int _y) : x{ _x }, y{ _y } {}
};

class Character
{
public:
    Character() = default;
    Character(int positionX_, int positionY_);
    ~Character() = default;

protected:
	Position position;
	vector<Position> bestPath;

	virtual bool move(Position) = 0;
	virtual void findBestPath(Position) = 0;

public:
    
};

#endif //MAPELEMENT_H
