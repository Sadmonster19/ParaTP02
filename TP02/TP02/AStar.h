#ifndef ASTAR_H
#define ASTAR_H

#include <set>
#include <vector>

#include "Character.h"
#include "RatHunter.h"

class AStar
{
public:
	static vector<Position> findBestPath(Character*, vector<Position>);
	static vector<Position> findAllElementInZone(RatHunter, vector<Position>);

private:
	static vector<Position> buildPath(std::shared_ptr<Node>);
	static bool isGoal(Position, vector<Position>);
    static bool isObstacle(MapObject);

	static vector<Position> getAllPositionForMapObject(MapStructure, MapObject);
	static int findClosestDistanceToGoal(Position, vector<Position>);

	static bool doesSetContains(SetNodePtr&, std::shared_ptr<Node>);

	static void removeBiggestScoreForNode(SetNodePtr&, std::shared_ptr<Node>);
};

#endif //ASTAR_H

