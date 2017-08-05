#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <algorithm>

enum Direction {
	NORTH_WEST,
	NORTH,
	NORTH_EAST,
	EAST,
	SOUTH_EAST,
	SOUTH,
	SOUTH_WEST,
	WEST
};

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

class Movement
{

public:
	Movement() = delete;
	~Movement() = delete;

	static Position getPositionForDirection(Direction);
};

#endif //MOVEMENT_H
