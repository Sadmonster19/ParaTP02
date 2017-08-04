#include "WorldMap.h"
#include <regex>

WorldMap::WorldMap()
    : ratsCount{}, ratHuntersCount{}
{

}

WorldMap::WorldMap(string fileName)
    : ratsCount{}, ratHuntersCount{}
{
    getInitialMap(fileName);
}

void WorldMap::getInitialMap(string mapName) {
    string temp = string(
        istreambuf_iterator<char>{ifstream{ MAPFILE_ROOT + mapName, ios::binary }},
        istreambuf_iterator<char>{}
    );

    fillMapStructure(temp);
}

void WorldMap::fillMapStructure(string mapString) {
    stringstream sstr;
    sstr << noskipws << mapString;

    istream_iterator<char> first{ sstr };
    istream_iterator<char> last{};

    vector<MapObject> temp;
    while (first != last) {
        switch (*first) 
        {
        case '#':
            temp.push_back(wall);
            break;
        case 'R':
            temp.push_back(rat);
            ratsCount++;
            break;
        case 'C':
            temp.push_back(ratHunter);
            ratHuntersCount++;
            break;
        case 'F':
            temp.push_back(cheese);
            break;
        case ' ':
            temp.push_back(emptySpace);
            break;
        case '\r':
            break;
        default:
            mapData.push_back(temp);
            temp.clear();
            break;
        }
        first++;
    }
}

vector<Position> WorldMap::getRatsPosition() {
    vector<Position> rats;
    
    for (unsigned int y = 0; y < mapData.size(); y++) {
        for (unsigned int x = 0; x < mapData[y].size(); x++)
            if (mapData[y][x] == rat) {
                Position p{ x, y };
                rats.push_back(p);
            }
    }
    return rats;
}

vector<Position> WorldMap::getRatHuntersPosition() {
    vector<Position> ratHunters;

    for (unsigned int y = 0; y < mapData.size(); y++) {
        for (unsigned int x = 0; x < mapData[y].size(); x++)
            if (mapData[y][x] == ratHunter) {
                Position p{ x, y };
                ratHunters.push_back(p);
            }
    }
    return ratHunters;
}

void WorldMap::displayMap() {
    string map{};

    for (unsigned int y = 0; y < mapData.size(); y++) {
        for (unsigned int x = 0; x < mapData[y].size(); x++) {
            switch (mapData[y][x])
            {
            case wall:
                map += '#';
                break;
            case rat:
                map += 'R';
                break;
            case ratHunter:
                map += 'C';
                break;
            case cheese:
                map += 'F';
                break;
            case emptySpace:
                map += ' ';
                break;
            default:
                break;
            }
        }
        map += '\n';
    }
    cout << map;
}

bool WorldMap::isGameDone() {
    return gameDone;
}

void WorldMap::endGame(bool done) {
    gameDone = done;
}

void WorldMap::initCharacters() {
    vector<Position> ratsPosition = getRatsPosition();
    vector<Position> ratHuntersPosition = getRatHuntersPosition();

    int id = 1;
    for (auto pos : ratsPosition) {
        int ratInfo[3] = {rat, pos.x, pos.y};

        MPI_Send(ratInfo, 1, MPI_2INT, id, 0, MPI_COMM_WORLD);
        id++;
    }
    for (auto pos : ratHuntersPosition) {
        int ratHunterInfo[3] = { ratHunter, pos.x, pos.y };

        MPI_Send(ratHunterInfo, 1, MPI_2INT, id, 0, MPI_COMM_WORLD);
        id++;
    }
}

MapObject WorldMap::getMapElement(Position pos) {
	return mapData[pos.y][pos.x];
}

void WorldMap::changeElement(Position pos, MapObject obj) {
	mapData[pos.y][pos.x] = obj;
}

void WorldMap::swapElements(Position pos1, Position pos2) {
	MapObject temp = getMapElement(pos1);
	changeElement(pos1, getMapElement(pos2));
	changeElement(pos2, temp);
}

bool WorldMap::moveCharacter(Position start, Position goal) {
	MapObject character = getMapElement(start);
	MapObject g = getMapElement(goal);

	bool success = false;

	switch (g)
	{
	case rat:
		if (character == ratHunter) {
			//TODO: Capture Rat -- I guess we just kill it?
			//changeElement(goal, emptySpace); -> Replace Rat with EmptySpace
			success = true;
		}
		break;
	case ratHunter:
		if (character == rat) {
			//TODO: Capture Rat -- I guess we just kill it?
			//changeElement(start, emptySpace); -> Replace Rat with EmptySpace
			success = true;
		}
		break;
	case cheese:
		if (character == rat) {
			//Eat cheese -- Replace cheese with EmptySpace
			changeElement(goal, emptySpace);
			success = true;
		}
		break;
	case emptySpace:
		success = true;
		break;
	default:
		break;
	}

	if (success)
		swapElements(start, goal);

	return success;
}
