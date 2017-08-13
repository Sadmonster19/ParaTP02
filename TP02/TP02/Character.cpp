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

void Character::setPosition(Position p) {
	position = p;
}

vector<Position> Character::getPossibleMovement() {
	return possibleMovement;
}

MapStructure Character::getInitialMap() {
	return initialMap;
}

bool Character::canWalkOn(Position p) {
    if (isOutOfBounds(p))
        return false;

    return initialMap[p.y][p.x] != MapObject::WALL;
}

Position Character::findNextMovement(vector<Position> goals) {
    return AStar::findBestNextMovement(this, goals);
}

void Character::setInitialMap(int id) {
    int height;
    int width;

    int mapInfo[2];
    MPI_Recv(&mapInfo, _countof(mapInfo), MPI_INT, 0, id, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    height = mapInfo[0];
    width = mapInfo[1];

    int arrayDim = height*width;

    int* map = new (int[arrayDim]);

    MPI_Recv(map, arrayDim, MPI_INT, 0, id, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    int i = 0;
    for (int y = 0; y < height; ++y) {
        vector<MapObject> line;
        for (int x = 0; x < width; ++x) {
            line.push_back(static_cast<MapObject>(map[i]));
            i++;
        }
        initialMap.push_back(line);
    }
}

vector<Position> Character::getMapObjectPositions(MapObject object, int id) {
    vector<Position> objectPositions;
    int arrayDim;
    int obj = static_cast<int>(object);

    MPI_Send(&obj, 1, MPI_INT, 0, id, MPI_COMM_WORLD);

    MPI_Recv(&arrayDim, 1, MPI_INT, 0, id, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    int* positions = new (int[arrayDim]);
    MPI_Recv(positions, arrayDim, MPI_INT, 0, id, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    for (int i = 0; i < arrayDim; i += 2) {
        objectPositions.push_back(Position{ positions[i], positions[i + 1] });
    }

    return objectPositions;
}

void Character::displayMap() {
    string map{};

    for (size_t y = 0; y < initialMap.size(); y++) {
        for (size_t x = 0; x < initialMap[y].size(); x++) {
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

bool Character::isOutOfBounds(Position p) {
	return  (p.x < 0 || p.x >= initialMap.at(0).size() || p.y < 0 || p.y >= initialMap.size());
}