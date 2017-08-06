#ifndef ASTAR_H
#define ASTAR_H

#include <set>
#include <vector>

#include "Character.h"
#include "RatHunter.h"

class AStar
{
public:
	static vector<Position> findBestPath(Character*, MapObject);
	static vector<Position> findAllElementInZone(RatHunter, vector<Position>);

private:
	static vector<Position> buildPath(std::shared_ptr<Node>);
	static bool isGoal(MapObject, MapObject);
    static bool isObstacle(MapObject);
	static bool isRat(vector<Position> rats, Position p);

	static vector<Position> getAllPositionForMapObject(MapStructure, MapObject);
	static int findClosestDistanceToGoal(Position, vector<Position>);

	static bool doesSetContains(SetNodePtr&, std::shared_ptr<Node>);

	static void removeBiggestScoreForNode(SetNodePtr&, std::shared_ptr<Node>);
};

#endif //ASTAR_H

