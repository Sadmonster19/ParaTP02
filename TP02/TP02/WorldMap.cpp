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
	findDoors();
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
			case door:
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
    for (auto position : ratsPosition) {
		th.emplace_back(async([&, id, position]() {
			Position pos = position;
			bool isAlive = true;
			//Send information to initiate the rat
			unsigned int ratInfo[2] = { pos.x, pos.y };
			MPI_Send(ratInfo, _countof(ratInfo), MPI_2INT, id, 0, MPI_COMM_WORLD);

			//Wait untile the game is started
			while (!gameReady);

			//Tell rat the game has started
			int start[1] = { 1 };
			MPI_Send(start, _countof(start), MPI_2INT, id, 0, MPI_COMM_WORLD);

			while (!gameDone && isAlive) {
				//Ask for the rat movement
				unsigned int movement[2];
				MPI_Recv(&movement, _countof(movement), MPI_2INT, id, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
				Position goal(movement[0], movement[1]);

				bool success = moveCharacter(pos, goal, isAlive);
				
				if (success) pos = goal;

				//Return to the rat the result of the move request
				int response[3] = { success, gameDone, isAlive };
				MPI_Send(response, _countof(response), MPI_2INT, id, 0, MPI_COMM_WORLD);
			}
			if (gameDone) {
				int response[3] = { false, gameDone, isAlive };
				MPI_Send(response, _countof(response), MPI_2INT, id, 0, MPI_COMM_WORLD);
			}
		}));
        id++;
    }
    for (auto pos : ratHuntersPosition) {
        int ratHunterInfo[3] = { ratHunter, pos.x, pos.y };

        MPI_Send(ratHunterInfo, _countof(ratHunterInfo), MPI_2INT, id, 0, MPI_COMM_WORLD);
        id++;
    }

	gameReady = true;

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

bool WorldMap::moveCharacter(Position start, Position goal, bool& isAlive) {
	lock_guard<std::mutex> lock(m);
	MapObject character = getMapElement(start);

	bool success = false;

	switch (getMapElement(goal))
	{
	case rat:
		if (character == ratHunter) {
			changeElement(goal, emptySpace);
			swapElements(start, goal);
			isAlive = false;
			success = true;
		}
		break;
	case ratHunter:
		if (character == rat) {
			changeElement(start, emptySpace);
			isAlive = false;
			success = true;
		}
		break;
	case cheese:
		if (character == rat) {
			//Eat cheese -- Replace cheese with EmptySpace
			changeElement(goal, emptySpace);
			swapElements(start, goal);
			success = true;
		}
		break;
	case emptySpace:
		swapElements(start, goal);
		success = true;
		break;
	case door:
		if (character == rat) {
			//Rat is out of the game -- Replace rat with EmptySpace
			changeElement(start, emptySpace);
			isAlive = false;
			success = true;
		}
		break;
	default:
		break;
	}

	if(success)
		displayMap();

	return success;
}

void WorldMap::findDoors() {
	size_t lineCount = mapData.size();
	//Doors can be on first line
	for (size_t i = 0; i < mapData[0].size(); ++i) {
		if (mapData[0][i] == emptySpace)
			mapData[0][i] = door;
	}

	//Doors can be on last line
	for (size_t i = 0; i < mapData[mapData.size()-1].size()-1; ++i) {
		if (mapData[lineCount-1][i] == emptySpace)
			mapData[lineCount-1][i] = door;
	}
	//Doors can be on first or last of each line
	for (size_t i = 1; i < mapData.size()-1; ++i) {
		size_t lineSize = mapData[i].size();

		if (mapData[i][0] == emptySpace)
			mapData[i][0] = door;
		if (mapData[i][lineSize-1] == emptySpace)
			mapData[i][lineSize-1] = door;
	}
}
