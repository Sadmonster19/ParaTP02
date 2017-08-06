#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <algorithm>
#include <memory>
#include <set>
#include <vector>
#include <limits>

using namespace std;

enum MapObject {
    WALL,
    RAT,
    HUNTER,
    CHEESE,
    EMPTY,
	DOOR
};

typedef vector<vector<MapObject>> MapStructure;

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

	static Position getPositionForDirection(Direction d) {
		Position p;

		switch (d) {
		case NORTH_WEST:
			p = Position{ 1,-1 };
			break;

		case NORTH:
			p = Position{ 1,0 };
			break;

		case NORTH_EAST:
			p = Position{ 1,1 };
			break;

		case EAST:
			p = Position{ 0,1 };
			break;

		case SOUTH_EAST:
			p = Position{ -1,1 };
			break;

		case SOUTH:
			p = Position{ -1,0 };
			break;

		case SOUTH_WEST:
			p = Position{ -1,-1 };
			break;

		case WEST:
			p = Position{ 0,-1 };
			break;

		default:
			throw new std::exception("No position associated for this direction");
		}

		return p;
	}
};

struct Node {
	int score;						//movementCost + distance from current to goal (with/without obstacle)
	Position p;						//Current position
	int movementCost;				//Cost from the beginning tile to the current
	std::shared_ptr<Node> parent;	//Needed to reconstruct the path

	Node(Position _p)
		: p{ _p } 
	{}

	Node(int _score, Position _p, int _movementCost, std::shared_ptr<Node> _parent)
		: score{ _score }, p{ _p }, movementCost{ _movementCost }, parent{ _parent }
	{}

	bool operator==(const Node& n) {
		return p == n.p;
	}
};

//Needed a Compare structure to have an ordered set based on the score and eliminate duplicates
struct Compare {
	bool operator()(std::shared_ptr<Node> a, std::shared_ptr<Node> b) const {
		return a->score < b->score;
	}
};

typedef set<std::shared_ptr<Node>, Compare> SetNode;

#endif //TOOLS_H
