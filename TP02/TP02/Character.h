#ifndef MAPELEMENT_H
#define MAPELEMENT_H

#include <iostream>
#include <mpi.h>
#include <vector>

using namespace std;

struct Position
{
    int x;
    int y;

	Position()
	{}

	Position(int _x, int _y)
		: x{ _x }, y{ _y }
	{}

	bool operator==(const Position& p) {
		return (x == p.x && y == p.y);
	}

	Position operator+(const Position& p) {
		return Position{ x + p.x, y + p.y };
	}

	int getDistance(const Position p) {
		return std::abs(x - p.x) + std::abs(y - p.y);
	}
};

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

#endif //MAPELEMENT_H
