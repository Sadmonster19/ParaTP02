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

private:
	int panicLevel = 0;

public:
	void initPossibleMovement() override;
	bool move(Position) override;
};

#endif //RAT_H