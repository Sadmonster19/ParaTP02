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

using namespace std;

enum MapObject{
    wall,
    rat,
    ratHunter,
    cheese,
    emptySpace
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

private:
    int ratsCount;
    int ratHuntersCount;
    bool gameDone = false;

    void fillMapStructure(string mapString);
    void getInitialMap(string mapName);
public:
    string const MAPFILE_ROOT = "";
    vector<Rat*> initRats();
    vector<RatHunter*> initRatHunters();
    void displayMap();
    bool isGameDone();
    void endGame(bool done);
};

#endif //WORLDMAP_H