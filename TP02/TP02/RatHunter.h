#ifndef RATHUNTER_H
#define RATHUNTER_H

#include "Character.h"
#include <iostream>

using namespace std;

class RatHunter : Character
{
public:
    RatHunter() = default;
    RatHunter(unsigned int positionX_, unsigned int positionY_);
    ~RatHunter() = default;

private:
    MapStructure mapData;
public:
    bool move(Position) override;
    void findBestPath(Position) override;
    Position findClosestCheese();
};

#endif // !RATHUNTER_H
