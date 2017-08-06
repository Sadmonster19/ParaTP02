#ifndef RATHUNTER_H
#define RATHUNTER_H

#include "Character.h"

using namespace std;

class RatHunter : public Character
{
public:
    RatHunter() = default;
    RatHunter(Position);
    ~RatHunter() = default;

public:
	void initPossibleMovement() override;
    bool move(Position) override;
};

#endif // !RATHUNTER_H
