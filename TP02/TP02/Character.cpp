#include "Character.h"
#include "AStar.h"
#include <string>
#include <new>

Character::Character(Position p_)
    : position{ p_ }, initialMap{}
{
}

Position Character::getPosition() {
	return position;
}

vector<Position> Character::getPossibleMovement() {
	return possibleMovement;
}

vector<Position> Character::getBestPath() {
	return bestPath;
}

void Character::findBestPath(MapObject mo) {
	bestPath = AStar::findBestPath(this, mo);
}

void Character::setInitialMap() {
    int height;
    int width;
    
    int mapInfo[2];
    MPI_Recv(&mapInfo, _countof(mapInfo), MPI_2INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    height = mapInfo[0];
    width = mapInfo[1];
    
    int arrayDim = height*width;
    cout << arrayDim << endl;

    int* map = new (int[arrayDim]);
    
    MPI_Recv(map, arrayDim, MPI_2INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    cout << height << "x" << width << endl;
    
    int i = 0;
    for (int y = 0; y < height; ++y) {
        vector<MapObject> line;
        for (int x = 0; x < width; ++x) {
            line.push_back(static_cast<MapObject>(map[i]));
            i++;
        }
        initialMap.push_back(line);
    }
    cout << i << endl;
    cout << "marde" << endl;
}

MapStructure Character::getInitialMap() {
    return initialMap;
}

void Character::displayMap() {
    string map{};

    for (unsigned int y = 0; y < initialMap.size(); y++) {
        for (unsigned int x = 0; x < initialMap[y].size(); x++) {
            switch (initialMap[y][x])
            {
            case WALL:
                map += '#';
                break;
            case RAT:
                map += 'R';
                break;
            case HUNTER:
                map += 'C';
                break;
            case CHEESE:
                map += 'F';
                break;
            case EMPTY:
                map += ' ';
                break;
            case DOOR:
                map += 'D';
                break;
            default:
                break;
            }
        }
        map += '\n';
    }
    cout << map << endl;
}