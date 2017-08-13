
#include <fstream>
#include <iostream>

#include "Stats.h"

mutex Stats::m = {};
map<int, int> Stats::totalMouvement = {};
map<int, int> Stats::acceptedMouvement = {};
map<int, int> Stats::capturedRat = {};
map<int, Position> Stats::fledRat = {};
map<int, Position> Stats::eatenCheese = {};
map<int, Position> Stats::screamedFrom = {};

void Stats::addTotalMouvement(int id) {
	if (totalMouvement.find(id) == totalMouvement.end())
		totalMouvement[id] = 0;

	std::lock_guard<mutex> lock(m);
	totalMouvement[id]++;
}

void Stats::addAcceptedMouvement(int id) {
	if (acceptedMouvement.find(id) == acceptedMouvement.end())
		acceptedMouvement[id] = 0;

	std::lock_guard<mutex> lock(m);
	acceptedMouvement[id]++;
}

void Stats::addCapturedRat(int idHunter, int idRat) {
	std::lock_guard<mutex> lock(m);
	capturedRat[idHunter] = idRat;
}

void Stats::addFledRat(int id, Position p) {
	std::lock_guard<mutex> lock(m);
	fledRat[id] = p;
}

void Stats::addEatenCheese(int id, Position p) {
	std::lock_guard<mutex> lock(m);
	eatenCheese[id] = p;
}

void Stats::addScreamedFrom(int id, Position p) {
	std::lock_guard<mutex> lock(m);
	screamedFrom[id] = p;
}

void Stats::printAllStats() {
	ofstream ofs{ "Stats.txt" };

	printMouvementInfo(ofs);
	printRatInfo(ofs);
	printRatHunterInfo(ofs);
}

void Stats::printMouvementInfo(ofstream& ofs) {
	ofs << "Mouvement Stats" << endl;
	ofs << "---------------" << endl;

	for (auto entry : totalMouvement)
		ofs << "Le protagoniste " << entry.first << " a demandé " << entry.second << " mouvements.";

	for (auto entry : acceptedMouvement)
		ofs << "Le protagoniste " << entry.first << " a demandé " << entry.second << " mouvements.";

	for (auto entry : totalMouvement) {
		float average = static_cast<float>(acceptedMouvement[entry.first]) / static_cast<float>(entry.second);
		ofs << "Le protagoniste " << entry.first << " a un taux de demandes de " << average << "%.";
	}

	ofs << endl;
}

void Stats::printRatInfo(ofstream& ofs) {
	ofs << "Rat Stats" << endl;
	ofs << "---------------" << endl;

	for (auto entry : capturedRat)
		ofs << "Le rat " << entry.first << " a été capturé par " << entry.second << ".";

	for (auto entry : fledRat)
		ofs << "Le rat " << entry.first << " s'est enfuie à la position [" << entry.second.x << "," << entry.second.y << "].";

	for (auto entry : eatenCheese) {
		ofs << "Le rat " << entry.first << " a mangé le fromage à la position [" << entry.second.x << ", " << entry.second.y << "].";
	}

	ofs << endl;
}

void Stats::printRatHunterInfo(ofstream& ofs) {
	ofs << "Rat Hunter Stats" << endl;
	ofs << "---------------" << endl;

	for (auto entry : screamedFrom)
		ofs << "Le chat " << entry.first << " a miaulle a la position" << entry.second.x << ", " << entry.second.y << "].";

	ofs << endl;
}