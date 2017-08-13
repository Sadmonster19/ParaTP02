#ifndef WORLDMAP_H
#define WORLDMAP_H

#include "Tools.h"
#include <string>
#include <fstream>
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
	ofstream ofs;

    void fillMapStructure(string mapString);
    void getInitialMap(string mapName);
	MapObject getMapElement(Position);
	void changeElement(Position, MapObject);
	bool moveCharacter(int, Position, Position, bool&);
	void swapElements(Position, Position);
	void findDoors();
	int getRatIdFromPosition(Position pos);
	vector<Position> getMapObjectPositions(MapObject object);
	void sendMapObjectPositions(int id);
	void sendRatsInPanic(int id);
	vector<Position> getHunterScream(int id);
	void sendInitialMapToCharacter(int id);
	bool isGameDone();
	bool isRatAlive(int id);

public:
	void displayMap();
    void playMap();
	void endGame();
};

#endif //WORLDMAP_H