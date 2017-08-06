#include "AStar.h"

vector<Position> AStar::findBestPath(MapStructure ms, Character* c, MapObject goal) {
	SetNode open;
	SetNode close;
	open.insert(std::make_shared<Node>(0, c->getPosition(), 0, nullptr));

	vector<Position> goals = getAllPositionForMapObject(ms, goal);

	while (!open.empty()) {
		std::shared_ptr<Node> current = *open.begin();
		open.erase(current);
		close.insert(current);

		if (isGoal(ms[current->p.y][current->p.x], goal)) {
			return buildPath(current);
		}

		for (Position p : c->getPossibleMovement()) {
			Position nextP = current->p + p;

			if (!WorldMap::isObstacle(ms[nextP.y][nextP.x])) {
				int t = findClosestDistanceToGoal(nextP, goals);
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

vector<Position> AStar::buildPath(std::shared_ptr<Node> n) {
	vector<Position> path;

	while (n->parent != nullptr) {
		path.emplace_back(n->p);
		n = n->parent;
	}
	path.emplace_back(n->p);
	std::reverse(path.begin(), path.end());

	return path;
}

bool AStar::isGoal(MapObject current, MapObject goal) {
	return current == goal;
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

bool AStar::doesSetContains(SetNode& ns, std::shared_ptr<Node> toFind) {
	return std::find_if(ns.begin(), ns.end(), [&](std::shared_ptr<Node> const& n) {
		return *n == *toFind;
	}) != ns.end();
}

void AStar::removeBiggestScoreForNode(SetNode& ns, std::shared_ptr<Node> toFind) {
	auto it = std::find_if(ns.begin(), ns.end(), [&](std::shared_ptr<Node> const& n) {
		return *n == *toFind;
	});

	if (it != ns.end() && it->get()->score <= toFind->score) {
		ns.erase(it);
	}
}