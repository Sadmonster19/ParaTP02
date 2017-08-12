#ifndef ASTAR_H
#define ASTAR_H

#include <set>
#include <vector>

#include "Character.h"
#include "RatHunter.h"

class AStar
{
public:
	static Position findBestNextMovement(Character*, vector<Position>);
	static vector<Position> findAllRatsInZone(RatHunter, vector<Position>);

private:
	static bool isGoal(Position, vector<Position>);
	static Position findNextMovement(NodePtr);
	static void removeBiggestScoreForNode(DequeNodePtr&, NodePtr);

	static int findClosestDistanceToGoal(Position, vector<Position>);
	static bool doesDequeContains(DequeNodePtr&, NodePtr);
};

#endif //ASTAR_H

