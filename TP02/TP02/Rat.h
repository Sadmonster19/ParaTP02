#ifndef RAT_H
#define RAT_H

#include "Character.h"

using namespace std;

class Rat : public Character
{
public:
    Rat() = default;
    Rat(Position);
    ~Rat() = default;
	int panicLevel = 0;

public:
	void initPossibleMovement() override;
	Position move(int id);
    bool isNearRatHunter(vector<Position>);
    vector<Position> getRatsInPanic(int id);
};

#endif //RAT_H