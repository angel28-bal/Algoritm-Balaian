#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <float.h>
#include <unordered_set>
#include "Pipe.h"
#include "Station.h"


//Проверка на корректность вводимых данных
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



void menu() {
	cout << "1. Добавить трубу" << endl;
	cout << "2. Добавить компрессорную станцию" << endl;
	cout << "3. Просмотр всех объектов " << endl;
	cout << "4. Редактировать трубу " << endl;
	cout << "5. Редактировать компрессорную станцию " << endl;
	cout << "6. Сохранить " << endl;
	cout << "7. Загрузить " << endl;
	cout << "8. Найти объекты по фильтру " << endl;
	cout << "0. Выход " << endl;
}


void save(unordered_map<int, Pipe>& pipeline, unordered_map<int, Station>& stationsGroup, string filepath) {
	ofstream fout;
	fout.open(filepath);
	
	if (!fout) {
		cout << "Не получилось открыть файл!";
	} else {
		fout << pipeline.size() << " " << stationsGroup.size() << endl;
		for (auto& pipe: pipeline)
			pipe.second.savePipe(fout);
		for (auto& station: stationsGroup)
			station.second.saveStation(fout);
		fout.close();
	}
}

void load(unordered_map<int, Pipe>& pipeline, unordered_map<int, Station>& stationsGroup, string filepath) {
	ifstream fin;
	
	fin.open(filepath, ios::in);
	if (!fin) {
		cout << "Не получилось открыть файл!";
	} else {
		Pipe::MAX_PIPE_ID = 0;
		Station::MAX_STATION_ID = 0;
		Pipe newPipe;
		Station newStation;
		pipeline.clear();
		stationsGroup.clear();
		int pipeNumber, stationNumber;
		fin >> pipeNumber >> stationNumber;
		for (int i(0); i < pipeNumber; i++){
			newPipe.loadPipe(fin);
			pipeline.insert({newPipe.GetID(), newPipe});
			Pipe::MAX_PIPE_ID = (Pipe::MAX_PIPE_ID < newPipe.GetID() ? newPipe.GetID() : Pipe::MAX_PIPE_ID);
		}
		for (int i(0); i < stationNumber; i++){
			newStation.loadStation(fin);
			stationsGroup.insert({newStation.GetID(), newStation});
			Station::MAX_STATION_ID = (Station::MAX_STATION_ID < newStation.GetID() ? newStation.GetID() : Station::MAX_STATION_ID);
		}
		
		Pipe::MAX_PIPE_ID++;
		Station::MAX_STATION_ID++;

		fin.close();
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
	
	int number;
	unordered_map <int, Pipe> pipeline;
	unordered_map <int, Station> cs_group;

	while (true) {

		menu();
		cout << "Пункт номер: ";
		number = GetCorrectNumber(0, 8);
		cout << endl << endl;
		switch (number)
		{
		case 0:
			return 0;
			
		case 1:{	
			Pipe newPipe;
			cin >> newPipe;
			cout << "Труба добавлена" << endl;
			pipeline.insert({newPipe.GetID(), newPipe});
			break;
		}
		case 2:{
			Station newCS;
			cin >> newCS;
			cs_group.insert({newCS.GetID(), newCS});
			cout << "Компрессорная станция добавлена" << endl;
			break;
		}
		case 3:{
			cout << "Трубы: " << endl;
			for (auto& pipe: pipeline)
				cout << pipe.second << endl << endl;
			cout << "Компрессорные станции: " << endl;
			for (auto& station : cs_group)
				cout << station.second << endl << endl;
			
			break;
		}
		case 4:{
			if (pipeline.size() == 0){
				cout << "Редактирование не возможно: не была создана ни одна труба. Попробуйте снова."<<endl;
				break;
			}
			pipeEdit(pipeline);
			break;
		}
		case 5:{
			if (cs_group.size() == 0){
				cout << "Редактирование не возможно: не была создана ни одна компрессорная станция. Попробуйте снова."<<endl;
				break;
			}
			stationEdit(cs_group);
			break;
		}
		case 6:{
			string pathToFile;
			cout << "Введите название файла сохранения: ";
			cin >> pathToFile;
			save(pipeline, cs_group, pathToFile);
			break;
		}
		case 7:{
			string pathToFile;
			cout << "Введите название файла для загрузки: ";
			cin >> pathToFile;
			load(pipeline, cs_group, pathToFile);
			break;
		}
		case 8:{
			unordered_set<int> result;
			int number;
			cout << "Выберите объект для поиска (0 - если трубы, 1 - если компрессорные станции): ";
			number = GetCorrectNumber(0, 1);
			if (number == 0){
				searchPipes(pipeline, result);
				for (auto& pipe: result)
					cout << pipeline[pipe] << endl << endl;
			} else if (number == 1){
				searchStations(cs_group, result);
				for (auto& station: result)
					cout << cs_group[station] << endl << endl;
			}
		}
		default:
			break;
		}
	}
}

