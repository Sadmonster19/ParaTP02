#ifndef ASTAR_H
#define ASTAR_H

#include "WorldMap.h"
#include "Character.h"
#include "Tools.h"

#include <memory>
#include <set>


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

