#pragma once
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include "pipeline.h"
#include "cs.h"

using namespace std;

struct CS_connected
{
	int initial_cs_id, terminal_cs_id;
	pipeline* pipeline_id;
};

struct GTN_data
{
	pipelines_data pipe_struct;
	cs_data cs_struct;
	vector <CS_connected> connect_struct;
};

class GTN_class
{
public:
	// Френдзона
	friend ostream& operator<<(ostream& os, const GTN_class& some_network);
	friend ofstream& operator<<(ofstream& ofs, const vector<CS_connected>& connect_struct);
	friend GTN_class& operator>>(const GTN_data& gtn_struct, GTN_class& gtn);

	friend void component_of_graph(const GTN_data& gtn_struct);
	friend void topology_sort_by_dfs(const GTN_data& gtn_struct);
	friend void out_shortest_path(int initial_point, map<int, int>& copy_tmp_cs, GTN_class& gtn);
	friend void dijkstra_algorithm(GTN_class& gtn);
	friend bool breadth_first_search(GTN_class& gtn, map<int, int>& parent, int Source, int Stock);
	friend void ford_falkerson(GTN_class& gtn);

private:
	unordered_map <int, unordered_map <int, pipeline*>> adjacency_matrix, inv_adjacency_matrix;
	unordered_map <int, unordered_map <int, int>> flow_matrix;
};