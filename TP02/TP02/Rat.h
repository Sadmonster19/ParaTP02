#ifndef RAT_H
#define RAT_H

#include "Character.h"
#include <iostream>

using namespace std;

class Rat : Character
{
public:
    Rat() = default;
    Rat(unsigned int positionX_, unsigned int positionY_);
    ~Rat() = default;

private:

	int panicLevel = 0;

public:
	bool move(Position) override;
	void findBestPath(Position) override;
	Position findClosestCheese();
};

#endif //RAT_H