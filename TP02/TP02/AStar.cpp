#include "AStar.h"

Position AStar::findBestNextMovement(Character* c, vector<Position> goals) {
	SetNodePtr open;
	SetNodePtr close;
	open.insert(std::make_shared<Node>(0, c->getPosition(), 0, nullptr));

	while (!open.empty()) {
		std::shared_ptr<Node> current = *open.begin();
		open.erase(current);
		close.insert(current);

		if (isGoal(current->p, goals)) {
			return findNextMovement(current);
		}

		for (Position p : c->getPossibleMovement()) {
			Position nextP = current->p + p;

			if (c->canWalkOn(current->p)) {
				int nextScore = findClosestDistanceToGoal(nextP, goals) + current->movementCost + 1;

				std::shared_ptr<Node> nextN = std::make_shared<Node>(nextScore, nextP, current->movementCost + 1, current);

				removeBiggestScoreForNode(open, nextN);
				removeBiggestScoreForNode(close, nextN);

				if (!doesSetContains(open, nextN) && !doesSetContains(close, nextN))
					open.insert(nextN);
			}
		}
	}

	return Position{};
}

vector<Position> AStar::findAllRatsInZone(RatHunter rh, vector<Position> rats) {
	SetNode inSight;
	SetNode tests;
	tests.insert(Node(rh.getPosition()));

	for (int i = 0; i < rh.MAX_SIGHT; ++i) {
		SetNode temp;

		for (Node n : tests) {
			for (Position p : rh.getPossibleMovement()) {
				Position nextP = n.p + p;

				if (isGoal(nextP, rats))
					inSight.insert(nextP);

				if (!rh.canWalkOn(nextP))
					temp.insert(Node{ nextP });
			}
		}
		tests = temp;
	}

	return Tools::convertSetToVector(inSight);
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
    auto element = std::min_element(goals.begin(), goals.begin(), [&p](Position& p1, Position& p2) {
        return p.getDistance(p1) < p.getDistance(p2);
    });

    return p.getDistance(*element);
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