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

	const int ZONE_LENGTH = 10;

public:
	void initPossibleMovement() override;
    bool move(Position) override;

	vector<Position> findAllRatsInZone(vector<Position>);
};

#endif // !RATHUNTER_H
