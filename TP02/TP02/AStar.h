#ifndef ASTAR_H
#define ASTAR_H

#include "Character.h"

class AStar
{
public:
	static vector<Position> findBestPath(MapStructure, Character*, MapObject);

private:
	static vector<Position> buildPath(std::shared_ptr<Node>);
	static bool isGoal(MapObject, MapObject);
    static bool isObstacle(MapObject);

	static vector<Position> getAllPositionForMapObject(MapStructure, MapObject);
	static int findClosestDistanceToGoal(Position, vector<Position>);

	static bool doesSetContains(SetNode&, std::shared_ptr<Node>);
	static void removeBiggestScoreForNode(SetNode&, std::shared_ptr<Node>);
};

#endif //ASTAR_H

