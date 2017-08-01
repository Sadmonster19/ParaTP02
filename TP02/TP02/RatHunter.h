#ifndef RATHUNTER_H
#define RATHUNTER_H

#include "Character.h"
#include <iostream>

using namespace std;

class RatHunter : Character
{
public:
    RatHunter() = default;
    RatHunter(int positionX_, int positionY_);
    ~RatHunter() = default;

private:

};

#endif // !RATHUNTER_H
