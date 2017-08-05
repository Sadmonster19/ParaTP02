#ifndef RATHUNTER_H
#define RATHUNTER_H

#include "Character.h"
#include "Movement.h"

#include <iostream>

using namespace std;

class RatHunter : public Character
{
public:
    RatHunter() = default;
    RatHunter(unsigned int positionX_, unsigned int positionY_);
    ~RatHunter() = default;

private:
    //MapStructure mapData;
public:
	void initPossibleMovement() override;
    bool move(Position) override;
    void findBestPath(Position) override;
};

#endif // !RATHUNTER_H
