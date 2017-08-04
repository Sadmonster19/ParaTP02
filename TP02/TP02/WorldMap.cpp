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
                Rat r{ x, y };
                rats.push_back(&r);
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