#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <float.h>
#include <unordered_set>

using namespace std;
 
//Проверка на корректность вводимых данных
template <typename T>
T GetCorrectNumber(T min, T max) {
	T x;
	while (!(cin >> x) || cin.fail() || (std::cin.peek() != '\n') || (x < min) || (x > max)) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Введите корректные данные (число в пределах: " << min << "-" << max << "): ";
	}
	return x;
}

class Pipe {
public:
	static int MAX_PIPE_ID;
	string name;
	bool workingStatus;

	Pipe(){ // Конструктор
		id = MAX_PIPE_ID++;
		diameter = 0;
		length = 0;
		workingStatus = false;
	}

	friend istream& operator>> (istream& in, Pipe& pipe){
		in.clear();
		in.ignore(INT_MAX, '\n');
		cout << "Имя: ";
		getline(in, pipe.name);
		cout << "\nДиаметр: ";
		pipe.diameter = GetCorrectNumber(0.0, DBL_MAX);
		cout << "\nДлина: ";
		pipe.length = GetCorrectNumber(0.0, DBL_MAX);
		cout << "\nСтатус трубы: ";
		pipe.workingStatus = GetCorrectNumber(0, 1);
		return in;
	}

	friend ostream& operator<< (ostream& out, Pipe& pipe){
		out << "Труба: " << endl;
		out << "ID: " << pipe.id << endl;
		out << "Имя: " << pipe.name << endl;
		out << "Диаметр: " << pipe.diameter << endl;
		out << "Длина: " << pipe.length << endl; 
		if (pipe.workingStatus) {
			out << "Работает"<< endl; 
		} else {
			out << "В ремонте" << endl;
		}
		return out;
	}

	void redactPipe();
	void savePipe(ofstream& fout);
	void loadPipe(ifstream& fin);
	int GetID(){return id;}
	void SetWorkingStatus(bool status){workingStatus = status;}

private:
	int id;
	double diameter;
	double length;
};

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

	friend istream& operator>> (istream& in, Station& station){
		in.clear();
		in.ignore(INT_MAX, '\n');
		cout << "Имя: ";
		getline(in, station.name);
		cout << "\nКоличество цехов: ";
		station.count = GetCorrectNumber(1, INT_MAX);
		cout << "\nКоличество цехов в работе:";
		station.countInWork = GetCorrectNumber(1, station.count);
		cout << "\nЭффективность: ";
		station.efficiency = GetCorrectNumber(0.0, 1.0);
		return in;
	}

	friend ostream& operator<< (ostream& out, Station& station){
		out << "Компрессорная станция: " << endl; 
		out << "ID: " << station.id << endl;
		out << "Имя: " << station.name << endl;
		out << "Количество цехов: " << station.count << endl; 
		out << "Количество цехов в работе: " << station.countInWork << endl; 
		out << "Эфективность: " << station.efficiency << endl; 
		return out;
	}

	void redactStation();
	void saveStation(ofstream& fout);
	void loadStation(ifstream& fin);
	int GetID(){return id;}
	double GetUnusedShopsPercentage(){return ((double)(count - countInWork) / count);}

private:
	int id;
	int count;
	int countInWork;
	double efficiency;
};

template <typename T>
using filterPipes = bool (*) (Pipe& pipe, T param);

template <typename T>
using filterStations = bool (*) (Station& station, T param);

template <typename T> // Поиск трубу по параметру
vector<int> searchPipeByParam (unordered_map<int, Pipe>& pipeline, filterPipes<T> f, T param){
	vector<int> result;
	for (auto& pipe: pipeline)
		if (f(pipe.second, param))
			result.push_back(pipe.second.GetID());
	return result;
}

template <typename T>
vector<int> searchStationByParam (unordered_map<int, Station>& cs_group, filterStations<T> f, T param){
	vector<int> result;
	for (auto& station: cs_group)
		if (f(station.second, param))
			result.push_back(station.second.GetID());
	return result;
}

bool checkPipeName(Pipe& pipe, string name){return (pipe.name.find(name) != string::npos);}

bool checkPipeStatus(Pipe& pipe, bool status){return (pipe.workingStatus == status);}

bool checkStationName(Station& station, string name){return (station.name.find(name) != string::npos);}

bool checkStationUnusedShops(Station& station, double param){return (station.GetUnusedShopsPercentage() >= param);}

void searchPipes(unordered_map<int, Pipe>& pipeline, vector<int>& resultVector){
	int number;
	cout<<"Выберите фильтрацию (0 - по имени, 1 - по статусу трубы): ";
	number = GetCorrectNumber(0, 1);
	if (number == 0){
		string name;
		cout << "Введите имя для поиска труб: ";
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		getline(cin, name);
		resultVector = searchPipeByParam(pipeline, checkPipeName, name);
	} else if (number == 1) {
		bool status;
		cout << "\nВведите состояние трубы для поиска (0 - если она в ремонте, 1 - если она работает): ";
		status = GetCorrectNumber(0, 1);
		resultVector = searchPipeByParam(pipeline, checkPipeStatus, status);
	}
}

void searchStations(unordered_map<int, Station>& cs_group, vector<int>& resultVector){
	int number;
	cout<<"Выберите фильтрацию (0 - по имени, 1 - по проценту не задействованных цехов): ";
	number = GetCorrectNumber(0, 1);
	if (number == 0){
		string name;
		cout << "\nВведите имя для поиска компрессорных станций: ";
		cin >> name;
		resultVector = searchStationByParam(cs_group, checkStationName, name);
	} else if (number == 1) {
		double param;
		cout << "\nВведите процент не задействованных цехов для поиска (в долях от единицы): ";
		param = GetCorrectNumber(0.0, 1.0);
		resultVector = searchStationByParam(cs_group, checkStationUnusedShops, param);
	}
}

int Pipe::MAX_PIPE_ID = 0;
int Station::MAX_STATION_ID = 0;

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

void Pipe::redactPipe() {
	cout << "Состояние трубы: ";
	if (workingStatus) {
		cout << "Работает" << endl; 
	} else {
		cout << "В ремонте" << endl;
	}
	cout << "Статус трубы (0 - если труба в ремонте, 1 - если труба работает): ";
	workingStatus = GetCorrectNumber(0, 1);
	cout << "Труба успешно изменена!" << endl;
}

void Station::redactStation() {
	cout << "ID: " << id << endl;
	cout << "Количество цехов: " << count << endl;
	cout << "Рабочих цехов: " << countInWork << endl;
	cout << "Введите количество рабочих цехов: ";
	countInWork = GetCorrectNumber(0, count);
	cout << "Компрессорная станция успешно изменена!" << endl;
}

void Pipe::savePipe(ofstream& fout){
	fout << id << endl;
	fout << name << endl;
	fout << diameter << endl;
	fout << length << endl;
	fout << workingStatus << endl;
}

void Station::saveStation(ofstream& fout){
	fout << id << endl;
	fout << name << endl;
	fout << count << endl;
	fout << countInWork << endl;
	fout << efficiency << endl;
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

void Pipe::loadPipe(ifstream& fin){
	fin >> this->id;
	getline(fin, name, '\n');
	getline(fin, name, '\n');
	fin >> this->diameter;
	fin >> this->length;
	fin >> this->workingStatus;
}

void Station::loadStation(ifstream& fin){
	fin >> id;
	getline(fin, name, '\n');
	getline(fin, name, '\n');
	fin >> count;
	fin >> countInWork;
	fin >> efficiency; 
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
			
		case 1:
		{	
			Pipe newPipe;
			cin >> newPipe;
			cout << "Труба добавлена" << endl;
			pipeline.insert({newPipe.GetID(), newPipe});
			break;
		}
		case 2:
		{
			Station newCS;
			cin >> newCS;
			cs_group.insert({newCS.GetID(), newCS});
			cout << "Компрессорная станция добавлена" << endl;
			break;
		}
		case 3:
		{
			cout << "Трубы: " << endl;
			for (auto& pipe: pipeline)
				cout << pipe.second << endl << endl;
			cout << "Компрессорные станции: " << endl;
			for (auto& station : cs_group)
				cout << station.second << endl << endl;
			
			break;
		}
		case 4:
		{
			if (pipeline.size() == 0){
				cout << "Редактирование не возможно: не была создана ни одна труба. Попробуйте снова."<<endl;
				break;
			}
			int number;
			cout << "Выберите работу с одной трубой (введите 0) или с множеством (введите 1): ";
			number = GetCorrectNumber(0, 1);
			if (number == 0){
				int pipeID;
				cout << "Введите id трубы: ";
				pipeID = GetCorrectNumber(0, Pipe::MAX_PIPE_ID);
				if (pipeline.find(pipeID) == pipeline.end()){
					cout << "Труба с введённым id не была найдена"<<endl;
					break;
				}
				cout << endl;
				int choice;
				cout << "Выберите действие с трубой (0 - редактирование, 1 - удаление): ";
				choice = GetCorrectNumber(0, 1);
				if (choice == 0)
					pipeline[pipeID].redactPipe();
				else if (choice == 1)
					pipeline.erase(pipeline.find(pipeID));
				
			} else if (number == 1) {//Редактирование по фильтру
				int choicePackage;
				cout << "Введите \"0\" для редактирования труб по фильтру или \"1\" для ввода id труб для редактирования: ";
				choicePackage = GetCorrectNumber(0, 1);
				if (choicePackage == 0){
					bool statusToSet;
					cout << "\nСтатус для установки выбранным трубам (0 - \"в ремонте\", 1 - \"в работе\"): ";
					statusToSet = (bool)GetCorrectNumber(0, 1);
					vector<int> searchVector;
					searchPipes(pipeline, searchVector);
					for (auto& id: searchVector){
						pipeline[id].SetWorkingStatus(statusToSet);
					}
				} else if (choicePackage == 1){//Редактирование по выбору
					unordered_set<int> ids;
					cout << "Введите количество редактируемых труб: ";
					int idsAmount = GetCorrectNumber(0, Pipe::MAX_PIPE_ID);
					cout << "\nВведите id труб для редактирования: ";
					for (int i(0); i<idsAmount; i++){
						int id = GetCorrectNumber(0, Pipe::MAX_PIPE_ID - 1);
						if (pipeline.find(id) != pipeline.end())
							ids.insert(id);
						else {
							cout << "Труба с данным id не существует." << endl;
							i--;
						}
					}
					cout << "Статус для установки выбранным трубам (0 - \"в ремонте\", 1 - \"в работе\"): ";
					bool statusToSet = (bool)GetCorrectNumber(0, 1);
					for (auto& id: ids){
						pipeline[id].SetWorkingStatus(statusToSet);
					}
				}
			}
			break;
		}
		case 5:
		{
			if (cs_group.size() == 0){
				cout << "Редактирование не возможно: не была создана ни одна компрессорная станция. Попробуйте снова."<<endl;
				break;
			}
			
			int number;
			cout << "Выберите работу с одной компрессорной станцией (введите 0) или с множеством (введите 1): ";
			number = GetCorrectNumber(0, 1);
			if (number == 0){
				int stationID;
				cout << "\nВведите id компрессорной станции: ";
				stationID = GetCorrectNumber(0, Station::MAX_STATION_ID);
				if (cs_group.find(stationID) == cs_group.end()){
					cout << "Станция с введённым id не была найдена"<<endl;
					break;
				}
				cout << endl;
				int choice;
				cout << "Выберите действие со станцией (0 - редактирование, 1 - удаление): ";
				choice = GetCorrectNumber(0, 1);
				if (choice == 0)
					cs_group[stationID].redactStation();
				else if (choice == 1)
					cs_group.erase(cs_group.find(stationID));
				
			} else if (number == 1){
				
				int choicePackage;
				cout << "Введите \"0\" для редактирования станций по фильтру или \"1\" для ввода id станций для редактирования: ";
				choicePackage = GetCorrectNumber(0, 1);
				if (choicePackage == 0){
					vector<int> searchVector;
					searchStations(cs_group, searchVector);
					cout << "Отредактируйте выбранные станции."<<endl;
					for (auto& id: searchVector){
						cs_group[id].redactStation();
					}
				} else if (choicePackage == 1){
					unordered_set<int> ids;
					cout << "\nВведите количество редактируемых станций: ";
					int idsAmount = GetCorrectNumber(0, Station::MAX_STATION_ID);
					cout << "\nВведите id станций для редактирования: ";
					for (int i(0); i<idsAmount; i++){
						int id = GetCorrectNumber(0, Station::MAX_STATION_ID - 1);
						if (cs_group.find(id)!=cs_group.end())
							ids.insert(id);
						else {
							cout << "Станции с данным id не существует." << endl;
							i--;
						}
					}
					cout << "Отредактируйте выбранные станции."<<endl;
					for (auto& id: ids){
						cs_group[id].redactStation();
					}
				}
			}
			break;
		}
		case 6:
		{
			string pathToFile;
			cout << "Введите название файла сохранения: ";
			cin >> pathToFile;
			save(pipeline, cs_group, pathToFile);
			break;
		}
		case 7:
		{
			
			string pathToFile;
			cout << "Введите название файла для загрузки: ";
			cin >> pathToFile;
			load(pipeline, cs_group, pathToFile);
			break;
		}
		case 8:
		{
			vector<int> result;
			int number;
			cout << "Выберите объект для поиска (0 - если трубы, 1 - если компрессорные станции): ";
			number = GetCorrectNumber(0, 1);
			if (number == 0){
				searchPipes(pipeline, result);
				if (result.size()) {
					for (auto& pipe : result)
						cout << pipeline[pipe] << endl << endl;
				}
				else {
					cout << "Трубы не найдены";
				}
				
			} else if (number == 1){
				searchStations(cs_group, result);
				if (result.size()) {
					for (auto& station : result)
						cout << cs_group[station] << endl << endl;
				}
				else {
					cout << "КС не найдены";
				}
				
			}
		}
		default:
			break;
		
		cout << endl << endl;
		}
	}
}

