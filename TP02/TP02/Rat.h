#ifndef RAT_H
#define RAT_H

#include "Character.h"
#include <iostream>

using namespace std;

class Rat : Character
{
public:
    Rat() = default;
    Rat(int positionX_, int positionY_);
    ~Rat() = default;

private:
	bool move(int x, int y) override;
	void findBestPath() override;
};

#endif //RAT_H