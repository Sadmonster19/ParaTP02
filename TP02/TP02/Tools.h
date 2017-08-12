#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <algorithm>
#include <deque>
#include <memory>
#include <set>
#include <vector>

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

	Position(int x_, int y_)
		: x{ x_ }, y{ y_ }
	{}

	bool operator==(const Position& p) {
		return (x == p.x && y == p.y);
	}

	bool operator<(const Position& p) {
		return x < p.x || (x == p.x && y < p.y);
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

	Node(Position p_)
		: p{ p_ }
	{}

	Node(int score_, Position p_, int movementCost_, std::shared_ptr<Node> parent_)
		: score{ score_ }, p{ p_ }, movementCost{ movementCost_ }, parent{ parent_ }
	{}

	bool operator==(const Node& n) {
		return p == n.p;
	}
};

typedef std::shared_ptr<Node> NodePtr;
typedef deque<NodePtr> DequeNodePtr;
typedef deque<Node> DequeNode;

struct Compare {
	inline bool operator() (NodePtr l, NodePtr r)
	{
		return (l->score < r->score);
	}

	inline bool operator() (Node l, Node r)
	{
		return (l.movementCost < r.movementCost);
	}
};

struct Tools {
	static vector<Position> convertDequeToVector(DequeNode dn) {
		vector<Position> result;
		for (auto d : dn)
			result.emplace_back(d.p);

		return result;
	}
};


#endif //TOOLS_H
