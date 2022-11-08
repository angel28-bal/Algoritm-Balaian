#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

using namespace std;

class Station {
public:
	static int MAX_STATION_ID;
	string name;

	Station () {
		id = MAX_STATION_ID++;
		count = 0;
		countInWork = 0;
		efficiency = 0;
	}

	friend istream& operator>> (istream& in, Station& station);

	friend ostream& operator<< (ostream& out, Station& station);

	void redactStation();
	void saveStation(ofstream& fout);
	void loadStation(ifstream& fin);
	int GetID();
	double GetUnusedShopsPercentage();

private:
	int id;
	int count;
	int countInWork;
	double efficiency;
};

template <typename T>
using filterStations = bool (*) (Station& station, T param);

bool checkStationName(Station& station, string name);

bool checkStationUnusedShops(Station& station, double param);

void searchStations(unordered_map<int, Station>& cs_group, unordered_set<int>& resultSet);

void stationEdit(unordered_map<int, Station>& cs_group);