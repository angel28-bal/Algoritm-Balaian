#pragma once

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;

class Pipe {
public:
	static int MAX_PIPE_ID;
	string name;
	bool workingStatus;

	Pipe(){
		id = MAX_PIPE_ID++;
		diameter = 0;
		length = 0;
		workingStatus = false;
	}

	friend istream& operator>> (istream& in, Pipe& pipe);

	friend ostream& operator<< (ostream& out, Pipe& pipe);

	void redactPipe();
	void savePipe(ofstream& fout);
	void loadPipe(ifstream& fin);
	int GetID();
	void SetWorkingStatus(bool status);
    

private:
	int id;
	double diameter;
	double length;
};

bool checkPipeName(Pipe& pipe, string name);

bool checkPipeStatus(Pipe& pipe, bool status);

template <typename T>
using filterPipes = bool (*) (Pipe& pipe, T param);

void searchPipes(unordered_map<int, Pipe>& pipeline, unordered_set<int>& resultSet);

void pipeEdit(unordered_map<int, Pipe>& pipeline);