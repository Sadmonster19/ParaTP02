#ifndef WOLRDMAP_H
#define WORLDMAP_H

#include "Rat.h"
#include "RatHunter.h"
#include <mpi.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <thread>
#include <future>
#include <mutex>

using namespace std;

enum MapObject{
    wall,
    rat,
    ratHunter,
    cheese,
    emptySpace,
	door
};

typedef vector<vector<MapObject>> MapStructure;

//Doit contenir un thread pour chaque character de le map.
class WorldMap
{
public:
    WorldMap();
    WorldMap(string fileName);
    ~WorldMap() = default;
    MapStructure mapData;
	vector<future<void>> th;
	bool gameDone = false;
	bool gameReady = false;

private:
    int ratsCount;
    int ratHuntersCount;
	vector<Position> doors;
	mutex m;

    void fillMapStructure(string mapString);
    void getInitialMap(string mapName);
	MapObject getMapElement(Position);
	void changeElement(Position, MapObject);
	bool moveCharacter(Position, Position, bool&);
	void swapElements(Position, Position);
	void findDoors();

public:
    string const MAPFILE_ROOT = "";
    void initCharacters();
    
    vector<Position> getRatsPosition();
    vector<Position> getRatHuntersPosition();
    void displayMap();
    bool isGameDone();
    void endGame(bool done);
	void endGame() {
		gameDone = true;

		for (auto it = begin(th); it != end(th); it++) {
			it->get();
		}
	};
};

#endif //WORLDMAP_H