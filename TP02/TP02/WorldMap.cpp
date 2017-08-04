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

vector<Rat*> WorldMap::initRats() {
    vector<Rat*> rats;
    
    for (unsigned int y = 0; y < mapData.size(); y++) {
        for (unsigned int x = 0; x < mapData[y].size(); x++)
            if (mapData[y][x] == rat) {
                Rat rank{ x, y };
                rats.push_back(&rank);
            }
    }
    return rats;
}

vector<RatHunter*> WorldMap::initRatHunters() {
    vector<RatHunter*> ratHunters;

    for (unsigned int y = 0; y < mapData.size(); y++) {
        for (unsigned int x = 0; x < mapData[y].size(); x++)
            if (mapData[y][x] == ratHunter) {
                RatHunter ratH{ x, y };
                ratHunters.push_back(&ratH);
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

	if(success)
		swapElements(start, goal);

	return success;
}