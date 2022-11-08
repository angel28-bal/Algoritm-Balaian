#include <iostream>
#include <string>
#include "Station.h"
#include <fstream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

template <typename T>
T GetCorrectNumber(T min, T max) {
	T x;
	while (!(cin >> x) || cin.fail() || (std::cin.peek() != '\n') || (x < min) || (x > max)) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Введите корректные данные (число в пределах: " << min << "-" << max << "):";
	}
	return x;
};


int Station::MAX_STATION_ID = 0;

istream& operator>> (istream& in, Station& station){
	in.clear();
	in.ignore(INT_MAX, '\n');
	cout << "Имя: ";
	getline(in, station.name);
	while ((station.name == "") || (station.name == "\n")){
		cout << "Поле не может быть пустым. Введите имя: ";
		getline(in, station.name);
	}
	cout << "\nКоличество цехов: ";
	station.count = GetCorrectNumber(1, INT_MAX);
	cout << "\nКоличество цехов в работе:";
	station.countInWork = GetCorrectNumber(1, station.count);
	cout << "\nЭффективность: ";
	station.efficiency = GetCorrectNumber(0.0, 1.0);
	return in;
}

ostream& operator<< (ostream& out, Station& station){
	out << "Компрессорная станция: " << endl; 
	out << "ID: " << station.id << endl;
	out << "Имя: " << station.name << endl;
	out << "Количество цехов: " << station.count << endl; 
	out << "Количество цехов в работе: " << station.countInWork << endl; 
	out << "Эфективность: " << station.efficiency << endl; 
	return out;
}

void Station::redactStation() {
	cout << "ID: " <<id<< endl;
	cout << "Количество цехов: " << count << endl;
	cout << "Рабочих цехов: " << countInWork << endl;
	cout << "Введите количество рабочих цехов: ";
	countInWork = GetCorrectNumber(0, count);
	cout << "Компрессорная станция успешно изменена!" << endl;
}

void Station::saveStation(ofstream& fout){
	fout << id << endl;
	fout << name << endl;
	fout << count << endl;
	fout << countInWork << endl;
	fout << efficiency << endl;
}

void Station::loadStation(ifstream& fin){
	fin >> id;
	getline(fin, name, '\n');
	getline(fin, name, '\n');
	fin >> count;
	fin >> countInWork;
	fin >> efficiency; 
}

int Station::GetID(){
    return id;
}

double Station::GetUnusedShopsPercentage(){
    return ((double)(count - countInWork) / count);
}

bool checkStationName(Station& station, string name){
    return (station.name.find(name) != string::npos);
}

bool checkStationUnusedShops(Station& station, double param){
    return (station.GetUnusedShopsPercentage() >= param);
}

template <typename T>
unordered_set<int> searchStationByParam (unordered_map<int, Station>& cs_group, filterStations<T> f, T param){
	unordered_set<int> result;
	for (auto& station: cs_group)
		if (f(station.second, param))
			result.insert(station.second.GetID());
	return result;
}

void searchStations(unordered_map<int, Station>& cs_group, unordered_set<int>& resultSet){
	int number;
	cout<<"Выберите фильтрацию (0 - по имени, 1 - по проценту не задействованных цехов): ";
	number = GetCorrectNumber(0, 1);
	if (number == 0){
		string name;
		cout << "\nВведите имя для поиска компрессорных станций: ";
		cin >> name;
		resultSet = searchStationByParam(cs_group, checkStationName, name);
	} else if (number == 1) {
		double param;
		cout << "\nВведите процент не задействованных цехов для поиска (в долях от единицы): ";
		param = GetCorrectNumber(0.0, 1.0);
		resultSet = searchStationByParam(cs_group, checkStationUnusedShops, param);
	}
}

void stationEdit(unordered_map<int, Station>& cs_group){
	int number;
	unordered_set<int> idSet;
	cout << "Выберите работу с одной компрессорной станцией (введите 0) или с множеством (введите 1): ";
	number = GetCorrectNumber(0, 1);
	if (number == 0){
		int csID;
		cout << "Введите id станции: ";
		csID = GetCorrectNumber(0, Station::MAX_STATION_ID);
		while (cs_group.find(csID) == cs_group.end()){
			cout << "Станция с введённым id не была найдена"<<endl;
			csID = GetCorrectNumber(0, Station::MAX_STATION_ID);
		}
		idSet.insert(csID);
		cout << endl;
		
	} else if (number == 1) {
		searchStations(cs_group, idSet);
		if (idSet.size() == 0){
			cout << "Станции не были найдены" << endl;
			return;	
		}
		cout << "Найденные станции: " << endl;
		for (auto id: idSet)
			cout << cs_group[id] << endl;
		cout << "Выберите действие с множеством станций (0 - продолжить работу с найденными станциями, 1 - выбрать подмножество станций из найденных): ";
		int multiChoice = GetCorrectNumber(0, 1);
		if (multiChoice == 1){
			unordered_set<int> idChoice;
			cout << "Введите кол-во станций, с которые вы выберете для работы: ";
			int amountStations = GetCorrectNumber(0, (int)idSet.size());
			for (int i = 0; i < amountStations; i++){
				cout << "Введите id станции: ";
				int idToChoose = GetCorrectNumber(0, Station::MAX_STATION_ID - 1);
				while (idSet.find(idToChoose) == idSet.end()){
					cout << "Станция с введёным id не была найдена фильтром."<<endl;
					idToChoose = GetCorrectNumber(0, Station::MAX_STATION_ID - 1);
				}
				idChoice.insert(idToChoose);
			}
			idSet = idChoice;
		}
	}
	if (idSet.size() == 0){
		return;
	} else {
		int choice;
		cout << "Выберите действие со станцией (-ами) (0 - редактирование, 1 - удаление): ";
		choice = GetCorrectNumber(0, 1);
		if (choice == 0)
			for (auto id : idSet)
				cs_group[id].redactStation();
		else if (choice == 1)
			for (auto id: idSet)
				cs_group.erase(cs_group.find(id));
	}
}