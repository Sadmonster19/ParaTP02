#ifndef RATHUNTER_H
#define RATHUNTER_H

#include <vector>

#include "Character.h"

using namespace std;

class RatHunter : public Character
{
public:
    RatHunter() = default;
    RatHunter(Position);
    ~RatHunter() = default;

	const int MAX_SIGHT = 10;

public:
	void initPossibleMovement() override;
    Position move(int id);
    void RatHunter::sendSreams(vector<Position> ratsPositions, int id);

	vector<Position> findAllRatsInZone(vector<Position>);

    bool canWalkOn(Position p) override;
};

#endif // !RATHUNTER_H
