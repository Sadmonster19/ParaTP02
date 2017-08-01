#ifndef MAPELEMENT_H
#define MAPELEMENT_H

#include <iostream>

using namespace std;

class MapElement
{
public:
    MapElement() = default;
    MapElement(int positionX_, int positionY_);
    ~MapElement() = default;

private:
    int positionX;
    int positionY;

public:
    
};

#endif //MAPELEMENT_H
