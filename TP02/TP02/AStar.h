#ifndef ASTAR_H
#define ASTAR_H

#include "WorldMap.h"
#include "Character.h"
#include "Movement.h"

#include <memory>
#include <set>

struct Node {
	int score;						//movementCost + distance from current to goal (with/without obstacle)
	Position p;						//Current position
	int movementCost;				//Cost from the beginning tile to the current
	std::shared_ptr<Node> parent;	//Needed to reconstruct the path

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

class AStar
{
public:
	static vector<Position> findBestPath(MapStructure, Character*, MapObject);

private:
	static vector<Position> buildPath(std::shared_ptr<Node>);
	static bool isGoal(MapObject, MapObject);

	static vector<Position> getAllPositionForMapObject(MapStructure, MapObject);
	static int findClosestDistanceToGoal(Position, vector<Position>);

	static bool doesSetContains(SetNode&, std::shared_ptr<Node>);
	static void removeBiggestScoreForNode(SetNode&, std::shared_ptr<Node>);
};

#endif //ASTAR_H

