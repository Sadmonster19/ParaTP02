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
	static Position findNextMovement(std::shared_ptr<Node>);
	static void removeBiggestScoreForNode(SetNodePtr&, std::shared_ptr<Node>);

	static int findClosestDistanceToGoal(Position, vector<Position>);
	static bool doesSetContains(SetNodePtr&, std::shared_ptr<Node>);
};

#endif //ASTAR_H

