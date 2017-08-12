#ifndef WORLDMAP_H
#define WORLDMAP_H

#include "Tools.h"
#include <string>
#include <thread>
#include <mutex>
#include <map>

using namespace std;

class WorldMap
{
public:
    WorldMap();
    WorldMap(string fileName);
    ~WorldMap() = default;
    MapStructure mapData;
	vector<thread> th;
    map<int, vector<Position>> ratToAlert;
    map<int, Position> rats;
    vector<int> ratToKill;
	bool gameDone = false;
	bool gameReady = false;

private:
    int ratsCount;
    int ratHuntersCount;
	mutex m;

    void fillMapStructure(string mapString);
    void getInitialMap(string mapName);
	MapObject getMapElement(Position);
	void changeElement(Position, MapObject);
	bool moveCharacter(Position, Position, bool&);
	void swapElements(Position, Position);
	void findDoors();

public:
    void playMap();
    int getRatIdFromPosition(Position pos);
    vector<Position> getMapObjectPositions(MapObject object);
    void displayMap();
    void sendMapObjectPositions(int id);
    void sendRatsInPanic(int id);
    vector<Position> getHunterScream(int id);
    void sendInitialMapToCharacter(int id);
    bool isGameDone();
    bool isRatAlive(int id);
    void endGame(bool done);
	void endGame() {
		gameDone = true;

		for (auto it = begin(th); it != end(th); it++) {
			it->join();
		}
	};
};

#endif //WORLDMAP_H