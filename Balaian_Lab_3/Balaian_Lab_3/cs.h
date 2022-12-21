#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

class cs
{
public:
	friend ostream& operator<<(ostream& os, const cs& c);
	friend istream& operator>>(istream& is, cs& p);
	friend ofstream& operator<<(ofstream& ofs, const cs& p);
	friend ifstream& operator>>(ifstream& ifs, cs& p);

	string get_name_cs() const;
	double get_load() const;
	void set_workshops_on(int new_working); // Редактирование КС

	int get_cs_id() const;
	void set_cs_id(int max_cs_id);

private:
	int id_cs = 0; // ID компрессорной станции
	string name_cs; // Название коммпрессионой станции
	int workshops_all = 0; // Количество цехов
	int workshops_on = 0; // Количество цехов в работе
	double efficiency = 0; // Эффективность работы в процентах
};

struct cs_data
{
	unordered_map <int, cs> mp_cs;
	int max_id_cs = 0;
};