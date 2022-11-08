#include <iostream>
#include <float.h>
#include <string>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include "Pipe.h"


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


int Pipe::MAX_PIPE_ID = 0;

istream& operator>> (istream& in, Pipe& pipe){
	in.clear();
	in.ignore(INT_MAX, '\n');
	cout << "Имя: ";
	getline(in, pipe.name);
	while ((pipe.name == "") || (pipe.name == "\n")){
		cout << "Поле не может быть пустым. Введите имя: ";
		getline(in, pipe.name);
	}
	cout << "\nДиаметр: ";
	pipe.diameter = GetCorrectNumber(0.0, DBL_MAX);
	cout << "\nДлина: ";
	pipe.length = GetCorrectNumber(0.0, DBL_MAX);
	cout << "\nСтатус трубы: ";
	pipe.workingStatus = GetCorrectNumber(0, 1);
	return in;
}


ostream& operator<< (ostream& out, Pipe& pipe){
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

void Pipe::savePipe(ofstream& fout){
	fout << id << endl;
	fout << name << endl;
	fout << diameter << endl;
	fout << length << endl;
	fout << workingStatus << endl;
}

void Pipe::loadPipe(ifstream& fin){
	fin >> this->id;
	getline(fin, name, '\n');
	getline(fin, name, '\n');
	fin >> this->diameter;
	fin >> this->length;
	fin >> this->workingStatus;
}

int Pipe::GetID(){
    return id;
}

void Pipe::SetWorkingStatus(bool status){
    workingStatus = status;
}

bool checkPipeName(Pipe& pipe, string name){
    return (pipe.name.find(name) != string::npos);
}

bool checkPipeStatus(Pipe& pipe, bool status){
    return (pipe.workingStatus == status);
}

template <typename T>
unordered_set<int> searchPipeByParam (unordered_map<int, Pipe>& pipeline, filterPipes<T> f, T param){
	unordered_set<int> result;
	for (auto& pipe: pipeline)
		if (f(pipe.second, param))
			result.insert(pipe.second.GetID());
	return result;
}

void searchPipes(unordered_map<int, Pipe>& pipeline, unordered_set<int>& resultSet){
	int number;
	cout<<"Выберите фильтрацию (0 - по имени, 1 - по статусу трубы): ";
	number = GetCorrectNumber(0, 1);
	if (number == 0){
		string name;
		cout << "Введите имя для поиска труб: ";
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		getline(cin, name);
		resultSet = searchPipeByParam(pipeline, checkPipeName, name);
	} else if (number == 1) {
		bool status;
		cout << "\nВведите состояние трубы для поиска (0 - если она в ремонте, 1 - если она работает): ";
		status = GetCorrectNumber(0, 1);
		resultSet = searchPipeByParam(pipeline, checkPipeStatus, status);
	}
}

void pipeEdit(unordered_map<int, Pipe>& pipeline){
	int number;
	unordered_set<int> idSet;
	cout << "Выберите работу с одной трубой (введите 0) или с множеством (введите 1): ";
	number = GetCorrectNumber(0, 1);
	if (number == 0){
		int pipeID;
		cout << "Введите id трубы: ";
		pipeID = GetCorrectNumber(0, Pipe::MAX_PIPE_ID);
		while (pipeline.find(pipeID) == pipeline.end()){
			cout << "Труба с введённым id не была найдена"<<endl;
			pipeID = GetCorrectNumber(0, Pipe::MAX_PIPE_ID);
		}
		idSet.insert(pipeID);
		cout << endl;
		
	} else if (number == 1) {
		searchPipes(pipeline, idSet);
		if (idSet.size() == 0){
			cout << "Трубы не были найдены" << endl;
			return;	
		}
		cout << "Найденные трубы: " << endl;
		for (auto id: idSet)
			cout << pipeline[id] << endl;
				
		cout << "Выберите действие с множеством труб (0 - продолжить работу с найденными трубами, 1 - выбрать подмножество труб из найденных): ";
		int multiChoice = GetCorrectNumber(0, 1);
		if (multiChoice == 1){
			unordered_set<int> idChoice;
			cout << "Введите кол-во труб, с которые вы выберете для работы: ";
			int amountStations = GetCorrectNumber(0, (int)idSet.size());
			for (int i = 0; i < amountStations; i++){
				cout << "Введите id трубы: ";
				int idToChoose = GetCorrectNumber(0, Pipe::MAX_PIPE_ID - 1);
				while (idSet.find(idToChoose) == idSet.end()){
					cout << "Труба с введёным id не была найдена фильтром."<<endl;
					idToChoose = GetCorrectNumber(0, Pipe::MAX_PIPE_ID - 1);
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
		cout << "Выберите действие с трубой (-ами) (0 - редактирование, 1 - удаление): ";
		choice = GetCorrectNumber(0, 1);

		if (choice == 0){
			bool statusToSet;
			cout << "\nСтатус для установки выбранным трубам (0 - \"в ремонте\", 1 - \"в работе\"): ";
			statusToSet = (bool)GetCorrectNumber(0, 1);
			for (auto id : idSet)
				pipeline[id].SetWorkingStatus(statusToSet);
		}else if (choice == 1)
			for (auto id: idSet)
				pipeline.erase(pipeline.find(id));
	}
}