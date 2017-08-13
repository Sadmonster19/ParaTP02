#include "AStar.h"

#include <deque>

Position AStar::findBestNextMovement(Character* c, vector<Position> goals) {
	DequeNodePtr open;
	DequeNodePtr close;

	open.emplace_back(std::make_shared<Node>(0, c->getPosition(), 0, nullptr));

	while (!open.empty()) {
		NodePtr current = open.front();
		open.pop_front();
		close.push_back(current);

		if (isGoal(current->p, goals)) {
			return findNextMovement(current);
		}
		for (Position p : c->getPossibleMovement()) {
			Position nextP = current->p + p;

			if (c->canWalkOn(nextP)) {
				int i = (p.x == 0 || p.y == 0) ? 1 : 0;
				int nextScore = findClosestDistanceToGoal(nextP, goals) +current->movementCost + i;

				NodePtr nextN = std::make_shared<Node>(nextScore, nextP, current->movementCost + 1, current);

				removeBiggestScoreForNode(open, nextN);
				removeBiggestScoreForNode(close, nextN);

				if (!doesDequeContains(open, nextN) && !doesDequeContains(close, nextN))
					open.push_back(nextN);
			}
			
		}
		std::sort(open.begin(), open.end(), Compare());
	}

	return Position{};
}

vector<Position> AStar::findAllRatsInZone(RatHunter rh, vector<Position> rats) {
	DequeNode inSight;
	DequeNode tests;
	tests.push_back(Node(rh.getPosition()));

	for (int i = 0; i < rh.MAX_SIGHT; ++i) {
		DequeNode temp;

		for (Node n : tests) {
			for (Position p : rh.getPossibleMovement()) {
				Position nextP = n.p + p;

				if (isGoal(nextP, rats))
					inSight.push_back(nextP);

				if (!rh.canWalkOn(nextP))
					temp.push_back(Node{ nextP });
			}
		}

		std::sort(temp.begin(), temp.end(), Compare());
		tests = temp;
	}

	return Tools::convertDequeToVector(inSight);
}

Position AStar::findNextMovement(std::shared_ptr<Node> n) {
    Position p;
    while (n->parent != nullptr) {
        p = n->p;
        n = n->parent;
    }
    return p;
}

bool AStar::isGoal(Position current, vector<Position> goals) {
	return std::find(goals.begin(), goals.end(), current) != goals.end();
}

int AStar::findClosestDistanceToGoal(Position p, vector<Position> goals) {
	vector<float> distance;

	for (auto v : goals)
		distance.emplace_back(p.getDistance(v));

	return *std::min_element(distance.begin(), distance.end());
}

bool AStar::doesDequeContains(DequeNodePtr& dnp, NodePtr toFind) {
	return std::find_if(dnp.begin(), dnp.end(), [&](NodePtr const& n) {
		return *n == *toFind;
	}) != dnp.end();
}

void AStar::removeBiggestScoreForNode(DequeNodePtr& dnp, NodePtr toFind) {
	auto it = std::find_if(dnp.begin(), dnp.end(), [&](NodePtr const& n) {
		return n->p == toFind->p;
	});

	if (it != dnp.end() && it->get()->score >= toFind->score) {
		dnp.erase(it);
	}
}