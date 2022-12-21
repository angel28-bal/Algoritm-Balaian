#pragma once
#include <vector>
#include "pipeline.h"
#include "cs.h"
#include "gtn.h"

using namespace std;

int checker();
int checker(int a, int b);
bool checker_bool();

void help();

void add_object_pipe(struct pipelines_data& pipe_struct);
void show_object_pipe(const unordered_map <int, pipeline>& mp);

void add_object_cs(struct cs_data& cs_struct);
void show_object_cs(const unordered_map <int, cs>& mp);

void save_data(const GTN_data& gtn_struct);
void load_data(GTN_data& gtn_struct);

void offer_interacte(unordered_map <int, pipeline>& mp, vector<int>& found_objects);

ofstream& operator<<(ofstream& ofs, const vector<CS_connected>& connect_struct);
ifstream& operator>>(ifstream& ifs, GTN_data& gtn_struct);;