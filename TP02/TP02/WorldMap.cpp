#include "WorldMap.h"
#include "Rat.h"
#include "RatHunter.h"
#include <fstream>
#include <regex>
#include <sstream>
#include <mpi.h>
#include <new>


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
        istreambuf_iterator<char>{ifstream{ mapName, ios::binary }},
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
            temp.push_back(WALL);
            break;
        case 'R':
            temp.push_back(RAT);
            ratsCount++;
            break;
        case 'C':
            temp.push_back(HUNTER);
            ratHuntersCount++;
            break;
        case 'F':
            temp.push_back(CHEESE);
            break;
        case ' ':
            temp.push_back(EMPTY);
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
    
    for (size_t y = 0; y < mapData.size(); y++) {
        for (size_t x = 0; x < mapData[y].size(); x++)
            if (mapData[y][x] == RAT) {
                rats.push_back(Position { static_cast<int>(x), static_cast<int>(y) });
            }
    }
    return rats;
}

vector<Position> WorldMap::getRatHuntersPosition() {
    vector<Position> ratHunters;

    for (size_t y = 0; y < mapData.size(); y++) {
        for (size_t x = 0; x < mapData[y].size(); x++)
            if (mapData[y][x] == HUNTER) {
				ratHunters.push_back(Position{ static_cast<int>(x), static_cast<int>(y) });
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
		th.emplace_back([&, id, position]() {
			Position pos = position;
			bool isAlive = true;

			//Send information to initiate the rat
			int ratInfo[2] = { pos.x, pos.y };
			MPI_Send(ratInfo, _countof(ratInfo), MPI_INT, id, id, MPI_COMM_WORLD);

            cout << "test" << endl;

            sendInitialMapToCharacter(id);

			//Wait untile the game is started
			while (!gameReady);

			//Tell rat the game has started
			int start = 1;
			MPI_Send(&start, 1, MPI_INT, id, id, MPI_COMM_WORLD);

			while (!gameDone && isAlive) {
				//Ask for the rat movement
				unsigned int movement[2];
				MPI_Recv(&movement, _countof(movement), MPI_INT, id, id, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
				Position goal(movement[0], movement[1]);

				bool success = moveCharacter(pos, goal, isAlive);
				
				if (success) pos = goal;

				//Return to the rat the result of the move request
				int response[3] = { success, gameDone, isAlive };
				MPI_Send(response, _countof(response), MPI_INT, id, id, MPI_COMM_WORLD);
			}
			if (gameDone) {
				int response[3] = { false, gameDone, isAlive };
				MPI_Send(response, _countof(response), MPI_INT, id, id, MPI_COMM_WORLD);
			}
		});
        id++;
    }
    for (auto position : ratHuntersPosition) {
        th.emplace_back([&, id, position]() {
            Position pos = position;
            bool isAlive = true;

            //Send information to initiate the ratHunter
            int ratHunterInfo[2] = { pos.x, pos.y };
            MPI_Send(ratHunterInfo, _countof(ratHunterInfo), MPI_INT, id, id, MPI_COMM_WORLD);

            sendInitialMapToCharacter(id);

            //Wait untile the game is started
            while (!gameReady);

            //Tell ratHunter the game has started
            int start = 1;
            MPI_Send(&start, 1, MPI_INT, id, id, MPI_COMM_WORLD);

            while (!gameDone) {
                //Ask for the rat movement
                unsigned int movement[2];
                MPI_Recv(&movement, _countof(movement), MPI_INT, id, id, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
                Position goal(movement[0], movement[1]);

                bool success = moveCharacter(pos, goal, isAlive);

                if (success) pos = goal;

                //Return to the rat the result of the move request
                int response[3] = { success, gameDone, isAlive };
                MPI_Send(response, _countof(response), MPI_INT, id, id, MPI_COMM_WORLD);
            }
            if (gameDone) {
                int response[3] = { false, gameDone, isAlive };
                MPI_Send(response, _countof(response), MPI_INT, id, id, MPI_COMM_WORLD);
            }
        });
        id++;

        /*int ratHunterInfo[3] = { HUNTER, position.x, position.y };

        MPI_Send(ratHunterInfo, 1, MPI_INT, id, 0, MPI_COMM_WORLD);
        id++;*/
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
	case RAT:
		if (character == HUNTER) {
			changeElement(goal, EMPTY);
			swapElements(start, goal);
			isAlive = false;
			success = true;
		}
		break;
	case HUNTER:
		if (character == RAT) {
			changeElement(start, EMPTY);
			isAlive = false;
			success = true;
		}
		break;
	case CHEESE:
		if (character == RAT) {
			//Eat cheese -- Replace cheese with EmptySpace
			changeElement(goal, EMPTY);
			swapElements(start, goal);
			success = true;
		}
		break;
	case EMPTY:
		swapElements(start, goal);
		success = true;
		break;
	case DOOR:
		if (character == RAT) {
			//Rat is out of the game -- Replace rat with EmptySpace
			changeElement(start, EMPTY);
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
		if (mapData[0][i] == EMPTY)
			mapData[0][i] = DOOR;
	}

	//Doors can be on last line
	for (size_t i = 0; i < mapData[mapData.size()-1].size()-1; ++i) {
		if (mapData[lineCount-1][i] == EMPTY)
			mapData[lineCount-1][i] = DOOR;
	}
	//Doors can be on first or last of each line
	for (size_t i = 1; i < mapData.size()-1; ++i) {
		size_t lineSize = mapData[i].size();

		if (mapData[i][0] == EMPTY)
			mapData[i][0] = DOOR;
		if (mapData[i][lineSize-1] == EMPTY)
			mapData[i][lineSize-1] = DOOR;
	}
}

void WorldMap::sendInitialMapToCharacter(int id) {
    int height = mapData.size();
    int width = mapData[0].size();

    int mapInfo[2]{height, width};
    MPI_Send(mapInfo, _countof(mapInfo), MPI_INT, id, id, MPI_COMM_WORLD);

    int arrayDim = height*width;

    int* map = new (int [arrayDim]);
    int i = 0;
    for (size_t y = 0; y < mapData.size(); y++) {
        for (size_t x = 0; x < mapData[y].size(); x++) {
            if (mapData[y][x] != RAT && mapData[y][x] != HUNTER)
                map[i] = static_cast<int>(mapData[y][x]);
            else
                map[i] = static_cast<int>(EMPTY);
            i++;
        }
    }
    MPI_Send(map, arrayDim, MPI_INT, id, id, MPI_COMM_WORLD);
}