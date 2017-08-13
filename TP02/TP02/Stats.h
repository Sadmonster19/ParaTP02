#ifndef STATS_H
#define STATS_H

#include "Tools.h"

#include <map>
#include <mutex>

using namespace std;

class Stats 
{
private:
	static mutex m;

	static map<int, int> totalMouvement;
	static map<int, int> acceptedMouvement;
	static vector<std::pair<int, int>> capturedRat;
	static vector<std::pair<int, Position>> fledRat;
	static vector<std::pair<int, Position>> eatenCheese;
	static vector<std::pair<int, Position>> screamedFrom;

	static void printMouvementInfo(ofstream&);
	static void printRatInfo(ofstream&);
	static void printRatHunterInfo(ofstream&);

public:
	static void addTotalMouvement(int);
	static void addAcceptedMouvement(int);
	static void addCapturedRat(int, int);
	static void addFledRat(int, Position);
	static void addEatenCheese(int, Position);
	static void addScreamedFrom(int, Position);

	static void printAllStats();

};

#endif