#ifndef MAPELEMENT_H
#define MAPELEMENT_H

#include <iostream>

using namespace std;

class Character
{
public:
    Character() = default;
    Character(int positionX_, int positionY_);
    ~Character() = default;

protected:
    int positionX;
    int positionY;
public:
    
};

#endif //MAPELEMENT_H
