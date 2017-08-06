#include "AStar.h"

vector<Position> AStar::findBestPath(Character* c, MapObject goal) {
	SetNodePtr open;
	SetNodePtr close;
	open.insert(std::make_shared<Node>(0, c->getPosition(), 0, nullptr));

	vector<Position> goals = getAllPositionForMapObject(c->getInitialMap(), goal);

	while (!open.empty()) {
		std::shared_ptr<Node> current = *open.begin();
		open.erase(current);
		close.insert(current);

		if (isGoal(c->getMapObjectForPosition(current->p), goal)) {
			return buildPath(current);
		}

		for (Position p : c->getPossibleMovement()) {
			Position nextP = current->p + p;

			if (!isObstacle(c->getMapObjectForPosition(current->p))) {
				int nextScore = findClosestDistanceToGoal(nextP, goals) + current->movementCost + 1;

				std::shared_ptr<Node> nextN = std::make_shared<Node>(nextScore, nextP, current->movementCost + 1, current);

				removeBiggestScoreForNode(open, nextN);
				removeBiggestScoreForNode(close, nextN);

				if (!doesSetContains(open, nextN) && !doesSetContains(close, nextN))
					open.insert(nextN);
			}
		}
	}

	return vector<Position>{};
}

vector<Position> AStar::findAllElementInZone(RatHunter rh, vector<Position> rats) {
	SetNode inSight;
	SetNode tests;
	tests.insert(Node(rh.getPosition()));

	for (int i = 0; i < rh.MAX_SIGHT; ++i) {
		SetNode temp;

		for (Node n : tests) {
			for (Position p : rh.getPossibleMovement()) {
				Position nextP = n.p + p;

				if (isRat(rats, nextP))
					inSight.insert(nextP);

				if (!isObstacle(rh.getMapObjectForPosition(nextP))) 
					temp.insert(Node{ nextP });
			}
		}
		tests = temp;
	}

	return Tools::convertSetToVector(inSight);
}

vector<Position> AStar::buildPath(std::shared_ptr<Node> n) {
	vector<Position> path;

	while (n->parent != nullptr) {
		path.emplace_back(n->p);
		n = n->parent;
	}
	path.emplace_back(n->p);

	return path;
}

bool AStar::isGoal(MapObject current, MapObject goal) {
	return current == goal;
}

bool AStar::isObstacle(MapObject current) {
    return current == WALL;
}

bool AStar::isRat(vector<Position> rats, Position p) {
	for (Position r : rats)
		if (r == p)
			return true;

	return false;
}

vector<Position> AStar::getAllPositionForMapObject(MapStructure ms, MapObject mo) {
	vector<Position> result;

	for (size_t y = 0; y < ms.size(); ++y) {
		for (size_t x = 0; x < ms[y].size(); ++x) {
			if (ms[y][x] == mo)
				result.emplace_back(Position{ static_cast<int>(x), static_cast<int>(y) });
		}
	}

	return result;
}

int AStar::findClosestDistanceToGoal(Position p, vector<Position> goals) {
	int closest = std::numeric_limits<int>::max();

	for (Position g : goals)
		closest = min(closest, g.getDistance(p));

	return closest;
}

bool AStar::doesSetContains(SetNodePtr& ns, std::shared_ptr<Node> toFind) {
	return std::find_if(ns.begin(), ns.end(), [&](std::shared_ptr<Node> const& n) {
		return *n == *toFind;
	}) != ns.end();
}

void AStar::removeBiggestScoreForNode(SetNodePtr& ns, std::shared_ptr<Node> toFind) {
	auto it = std::find_if(ns.begin(), ns.end(), [&](std::shared_ptr<Node> const& n) {
		return *n == *toFind;
	});

	if (it != ns.end() && it->get()->score <= toFind->score) {
		ns.erase(it);
	}
}