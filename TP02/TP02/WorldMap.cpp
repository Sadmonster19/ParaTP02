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

vector<Position> WorldMap::getMapObjectPositions(MapObject object) {
    vector<Position> objectPositions;

    for (size_t y = 0; y < mapData.size(); y++) {
        for (size_t x = 0; x < mapData[y].size(); x++)
            if (mapData[y][x] == object) {
                objectPositions.push_back(Position{ static_cast<int>(x), static_cast<int>(y) });
            }
    }
    return objectPositions;
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

void WorldMap::playMap() {
    vector<Position> ratsPosition = getMapObjectPositions(RAT);
    vector<Position> ratHuntersPosition = getMapObjectPositions(HUNTER);

    int id = 1;
    for (auto position : ratsPosition) {
		th.emplace_back([&, id, position]() {
			Position pos = position;
			bool isAlive = true;

			//Send information to initiate the rat
			int ratInfo[2] = { pos.x, pos.y };
			MPI_Send(ratInfo, _countof(ratInfo), MPI_INT, id, id, MPI_COMM_WORLD);

            rats[id] = pos;

            sendInitialMapToCharacter(id);

			//Wait untile the game is started
			while (!gameReady);

			//Tell rat the game has started
			int start = 1;
			MPI_Send(&start, 1, MPI_INT, id, id, MPI_COMM_WORLD);

			while (!gameDone && isAlive) {
                isAlive = isRatAlive(id);
                sendRatsInPanic(id);

                //Get next move
                sendMapObjectPositions(id);

				//Ask for the rat movement
				unsigned int movement[2];
				MPI_Recv(&movement, _countof(movement), MPI_INT, id, id, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
				Position goal(movement[0], movement[1]);
				bool success = moveCharacter(pos, goal, isAlive);
				
                if (success) {
                    pos = goal;
                    lock_guard<mutex> lock(m);
                    rats[id] = pos;
                }

                gameDone = getMapObjectPositions(CHEESE).empty();

				
                //Return to the rat the result of the move request
				int response[3] = { success, gameDone, isAlive };
				MPI_Send(response, _countof(response), MPI_INT, id, id, MPI_COMM_WORLD);
                _sleep(1000);
			}
		});
        id++;
    }
    for (auto position : ratHuntersPosition) {
        th.emplace_back([&, id, position]() {
            Position pos = position;
            bool isAlive = true;

            //Send information to initiate the ratHunter
            int hunterInfo[2] = { pos.x, pos.y };
            MPI_Send(hunterInfo, _countof(hunterInfo), MPI_INT, id, id, MPI_COMM_WORLD);

            sendInitialMapToCharacter(id);

            //Wait untile the game is started
            while (!gameReady);

            //Tell ratHunter the game has started
            int start = 1;
            MPI_Send(&start, 1, MPI_INT, id, id, MPI_COMM_WORLD);

            while (!gameDone) {
                //Get next move
                sendMapObjectPositions(id);
                //sendMapObjectPositions(id);

                {
                    lock_guard<mutex> lock(m);
                    ratToAlert[id] = getHunterScream(id);
                }

                //Ask for the ratHunter movement
                unsigned int movement[2];
                MPI_Recv(&movement, _countof(movement), MPI_INT, id, id, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
                Position goal(movement[0], movement[1]);

                bool success = moveCharacter(pos, goal, isAlive);

                if (success) {
                    pos = goal;
                    if (!isAlive) {
                        lock_guard<mutex> lock(m);
                        int ratId = getRatIdFromPosition(pos);
                        if(ratId != 0)
                            ratToKill.push_back(ratId);
                    }
                }

                gameDone = (getMapObjectPositions(RAT).empty() || getMapObjectPositions(CHEESE).empty());

                //Return to the rat the result of the move request
                int response[2] = { success, gameDone };
                MPI_Send(response, _countof(response), MPI_INT, id, id, MPI_COMM_WORLD);
                _sleep(1000);
            }
        });
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
    if(!gameDone){
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

        if (success)
            displayMap();
    }

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

void WorldMap::sendMapObjectPositions(int id) {
    int object;
    MPI_Recv(&object, 1, MPI_INT, id, id, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
    
    vector<Position> objectPositions = getMapObjectPositions(static_cast<MapObject>(object));

    int arrayDim = objectPositions.size() * 2;
    int* positions = new (int[arrayDim]);

    MPI_Send(&arrayDim, 1, MPI_INT, id, id, MPI_COMM_WORLD);

    int i = 0;
    for (auto position : objectPositions) {
        positions[i] = position.x;
        positions[i+1] = position.y;
        i += 2;
    }

    MPI_Send(positions, arrayDim, MPI_INT, id, id, MPI_COMM_WORLD);
}

vector<Position> WorldMap::getHunterScream(int id) {
    vector<Position> ratsScreamedAt;
    int scream;

    MPI_Recv(&scream, 1, MPI_INT, id, id, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
    if (scream) {

        int arrayDim;
        MPI_Recv(&arrayDim, 1, MPI_INT, id, id, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        int* positions = new (int[arrayDim]);
        MPI_Recv(positions, arrayDim, MPI_INT, id, id, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 0; i < arrayDim; i += 2) {
            ratsScreamedAt.push_back(Position{ positions[i], positions[i + 1] });
        }
    }

    return ratsScreamedAt;
}

void WorldMap::sendRatsInPanic(int id) {
    int arrayDim = 0;

    for (auto rats : ratToAlert)
        arrayDim += rats.second.size();

    arrayDim *= 2;
    int* positions = new (int[arrayDim]);

    MPI_Send(&arrayDim, 1, MPI_INT, id, id, MPI_COMM_WORLD);

    int i = 0;
    for (auto rats : ratToAlert) {
        for (auto position : rats.second) {
            positions[i] = position.x;
            positions[i + 1] = position.y;
            i += 2;
        }
    }

    MPI_Send(positions, arrayDim, MPI_INT, id, id, MPI_COMM_WORLD);
}

int WorldMap::getRatIdFromPosition(Position pos) {
    for (auto it = rats.begin(); it != rats.end(); ++it) {
        if (it->second == pos)
            return it->first;
    }
    return 0;
}

bool WorldMap::isRatAlive(int id) {
    lock_guard<mutex> lock(m);
    auto it = std::find(ratToKill.begin(), ratToKill.end(), id);
    if (it != ratToKill.end()) {
        ratToKill.erase(it);
        return false;
    }
    else
        return true;
}