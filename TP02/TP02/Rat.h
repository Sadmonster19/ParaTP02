#ifndef RAT_H
#define RAT_H

#include "Character.h"
#include "Movement.h"

#include <iostream>

using namespace std;

class Rat : public Character
{
public:
    Rat() = default;
    Rat(unsigned int positionX_, unsigned int positionY_);
    ~Rat() = default;

private:
    //MapStructure mapData;
	int panicLevel = 0;

public:
	void initPossibleMovement() override;
	bool move(Position) override;
	void findBestPath(Position) override;
};

#endif //RAT_H