
#include <fstream>
#include <iostream>

#include "Stats.h"

mutex Stats::m = {};
map<int, int> Stats::totalMouvement = {};
map<int, int> Stats::acceptedMouvement = {};
vector<std::pair<int, int>> Stats::capturedRat = {};
vector<std::pair<int, Position>> Stats::fledRat = {};
vector<std::pair<int, Position>> Stats::eatenCheese = {};
vector<std::pair<int, Position>> Stats::screamedFrom = {};

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
	capturedRat.push_back(std::make_pair(idHunter, idRat));
}

void Stats::addFledRat(int id, Position p) {
	std::lock_guard<mutex> lock(m);
	fledRat.push_back(std::make_pair(id, p));
}

void Stats::addEatenCheese(int id, Position p) {
	std::lock_guard<mutex> lock(m);
	eatenCheese.push_back(std::make_pair(id, p));
}

void Stats::addScreamedFrom(int id, Position p) {
	std::lock_guard<mutex> lock(m);
	screamedFrom.push_back(std::make_pair(id, p));
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
		ofs << "Le protagoniste " << entry.first << " a demande " << entry.second << " mouvements." << endl;

	for (auto entry : acceptedMouvement)
		ofs << "Le protagoniste " << entry.first << " a reussis " << entry.second << " mouvements." << endl;

	for (auto entry : totalMouvement) {
		float average = static_cast<float>(acceptedMouvement[entry.first]) / static_cast<float>(entry.second) * 100;
		ofs << "Le protagoniste " << entry.first << " a un taux de reussite de " << average << "%." << endl;
	}

	ofs << endl;
}

void Stats::printRatInfo(ofstream& ofs) {
	ofs << "Rat Stats" << endl;
	ofs << "---------------" << endl;

	for (auto entry : capturedRat)
		ofs << "Le rat " << entry.first << " a ete capture par " << entry.second << "." << endl;

	for (auto entry : fledRat)
		ofs << "Le rat " << entry.first << " s'est enfuie a la position [" << entry.second.x << ", " << entry.second.y << "]." << endl;

	for (auto entry : eatenCheese) {
		ofs << "Le rat " << entry.first << " a mange le fromage a la position [" << entry.second.x << ", " << entry.second.y << "]." << endl;
	}

	ofs << endl;
}

void Stats::printRatHunterInfo(ofstream& ofs) {
	ofs << "Rat Hunter Stats" << endl;
	ofs << "---------------" << endl;

	for (auto entry : screamedFrom)
		ofs << "Le chasseur " << entry.first << " a miaulle a la position ­[" << entry.second.x << ", " << entry.second.y << "]." << endl;

	ofs << endl;
}