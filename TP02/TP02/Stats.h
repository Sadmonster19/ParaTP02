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
	static map<int, int> capturedRat;
	static map<int, Position> fledRat;
	static map<int, Position> eatenCheese;
	static map<int, Position> screamedFrom;

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