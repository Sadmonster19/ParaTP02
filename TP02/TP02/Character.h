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

	virtual bool move(int x, int y) = 0;
	virtual void findBestPath() = 0;

public:
    
};

#endif //MAPELEMENT_H
