#pragma once
#include "gtn.h"
#include "utility.h"

int checker_cs_id(const unordered_map <int, cs>& mp_cs);
int checker_pipeline_id(pipelines_data& struct_pipe);

void create_new_connection(vector <CS_connected>& connect_struct, pipelines_data& struct_pipe, const unordered_map <int, cs>& mp_cs);
void show_all_connection(const GTN_data& gtn_struct);

void component_of_graph(const GTN_data& gtn_struct);
void topology_sort_by_dfs(const GTN_data& gtn_struct);

void find_all_shortest_path(const GTN_data& gtn_struct);
void find_max_flow(const GTN_data& gtn_struct);