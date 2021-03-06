#ifndef WORLDMAP_H
#define WORLDMAP_H

#include "Tools.h"
#include <string>
#include <thread>
#include <mutex>

using namespace std;

class WorldMap
{
public:
    WorldMap();
    WorldMap(string fileName);
    ~WorldMap() = default;
    MapStructure mapData;
	vector<thread> th;
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
    void initCharacters();

    vector<Position> getRatsPosition();
    vector<Position> getRatHuntersPosition();
    void displayMap();
    void sendInitialMapToCharacter(int id);
    bool isGameDone();
    void endGame(bool done);
	void endGame() {
		gameDone = true;

		for (auto it = begin(th); it != end(th); it++) {
			it->join();
		}
	};
};

#endif //WORLDMAP_H