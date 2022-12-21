#include "gtn_utility.h"
#include <set>
#include <map>
#include <queue>
#include <stack>

using namespace std;

int checker_cs_id(const unordered_map <int, cs>& mp_cs)
{
	while (1) {
		int id = checker();
		if (!id) break;
		if (mp_cs.count(id))
			return(id);
		else
			cout << "\n This ID not exist. Try again or 0 to exit: ";
	}
}

int checker_pipeline_id(pipelines_data& struct_pipe)
{
	while (1) {
		cout << "\n Enter pipeline's diameter 500, 700, 1400: ";
		int diameter = checker();
		if (!((diameter == 500) || (diameter == 700) || (diameter == 1400))) {
			cout << endl << " Incorrect diameter. Try again. " << endl;
			continue;
		}

		for (auto iter_search : struct_pipe.mp_pipe)
			if (diameter == iter_search.second.get_diameter() && (!iter_search.second.get_used()) && (!iter_search.second.get_repair()))
				return(iter_search.first);
		while (1) {
			cout << "\n This pipeline diameter's not exist. Create new pipeline ? 1/0: ";
			if (checker(0, 1))
				add_object_pipe(struct_pipe);
			if (struct_pipe.mp_pipe[struct_pipe.max_id_pipeline].get_diameter() != diameter)
				cout << " This pipeline hasn't " << diameter << " diameter. Create again";
			else return(struct_pipe.max_id_pipeline);
		}
	}
	return(struct_pipe.max_id_pipeline);
}

void create_new_connection(vector <CS_connected>& connect_struct, pipelines_data& struct_pipe, const unordered_map <int, cs>& mp_cs)
{
	CS_connected new_connect;

	cout << "\n Enter initial compressor station's ID: ";
	new_connect.initial_cs_id = checker_cs_id(mp_cs);
	if (!new_connect.initial_cs_id) return;

	cout << "\n Enter terminal compressor station'S ID: ";
	new_connect.terminal_cs_id = checker_cs_id(mp_cs);
	if (!new_connect.terminal_cs_id) return;

	int id = checker_pipeline_id(struct_pipe);
	new_connect.pipeline_id = &struct_pipe.mp_pipe.at(id);
	if (!new_connect.pipeline_id) return;

	struct_pipe.mp_pipe[id].set_used(true);

	connect_struct.push_back(new_connect);
}

void show_all_connection(const GTN_data& gtn_struct)
{
	GTN_class gtn;
	gtn_struct >> gtn;
	cout << gtn;
}

void component_of_graph(const GTN_data& gtn_struct)
{
	GTN_class gtn;
	gtn_struct >> gtn;

	for (auto iter_adj_matrix : gtn.adjacency_matrix)
		for (auto iter_adj_matrix_at_cs_id : iter_adj_matrix.second)
			gtn.adjacency_matrix[iter_adj_matrix_at_cs_id.first][iter_adj_matrix.first] = iter_adj_matrix_at_cs_id.second;
	//cout << gtn;

	map <int, bool> visited_point;

	for (auto iter_adj_matrix : gtn.adjacency_matrix)
	{
		visited_point[iter_adj_matrix.first] = false;
		for (auto iter_adj_matrix_at_cs_id : iter_adj_matrix.second)
			visited_point[iter_adj_matrix_at_cs_id.first] = false;
	}

	auto first_cs_id = gtn.adjacency_matrix.begin();
	int ID_cs = first_cs_id->first;

	int num_component = 1;
	set <int> component_graph;
	component_graph.insert(ID_cs);
	stack <int> stack_dfs;
	stack_dfs.push(ID_cs);

	while (1) {
		while (!stack_dfs.empty())
		{
			ID_cs = stack_dfs.top();
			stack_dfs.pop();
			visited_point[ID_cs] = true;

			for (auto iter_adj_matrix : gtn.adjacency_matrix[ID_cs])
			{
				ID_cs = iter_adj_matrix.first;
				if (visited_point[ID_cs] == false)
				{
					stack_dfs.push(ID_cs);
					visited_point[ID_cs] = true;
					component_graph.insert(ID_cs);
				}
			}
		}

		cout << " Component " << num_component++ << " ->[";
		for (auto iter_component_graph : component_graph)
			cout << iter_component_graph << ",";
		cout << "]" << endl;
		component_graph.clear();

		for (auto iter_visited_point : visited_point)
			if (iter_visited_point.second == false)
			{
				component_graph.insert(iter_visited_point.first);
				stack_dfs.push(iter_visited_point.first);
				break;
			}
		if (stack_dfs.empty()) break;
	}
}

bool check_cycle_by_dfs(const unordered_map <int, unordered_map <int, pipeline*>>& adjacency_matrix)
{
	stack <int> stack_dfs;
	for (auto iter_adj_matrix : adjacency_matrix)
	{
		map <int, bool> visited_point;
		int ID_cs = 0;
		stack_dfs.push(iter_adj_matrix.first);

		while (!stack_dfs.empty())
		{
			ID_cs = stack_dfs.top();
			stack_dfs.pop();

			if (adjacency_matrix.find(ID_cs) != adjacency_matrix.end())
				for (auto& iter_adj_matrix_ID_cs : adjacency_matrix.at(ID_cs))
				{
					if (visited_point[iter_adj_matrix_ID_cs.first])
					{
						cout << " This graph isn't tree. Topology sort is impossible " << endl;
						return(1);
					}
					stack_dfs.push(iter_adj_matrix_ID_cs.first);
					visited_point[ID_cs] = true;
				}
		}
	}
	return(0);
}

void topology_sort_by_dfs(const GTN_data& gtn_struct)
{
	GTN_class gtn;
	gtn_struct >> gtn;

	map <int, bool> visited_point;
	vector<int> top_sort_cs;

	for (auto iter_adj_matrix : gtn.adjacency_matrix)
		if (gtn.inv_adjacency_matrix.find(iter_adj_matrix.first) == gtn.inv_adjacency_matrix.end())
		{
			int ID_cs = iter_adj_matrix.first;
			stack <int> stack_dfs;

			stack_dfs.push(ID_cs);
			top_sort_cs.push_back(ID_cs);
			while (!stack_dfs.empty())
			{
				ID_cs = stack_dfs.top();
				stack_dfs.pop();
				for (auto iter_adj_matrix : gtn.adjacency_matrix[ID_cs])
				{
					ID_cs = iter_adj_matrix.first;
					stack_dfs.push(ID_cs);
					if (!visited_point[ID_cs])
						top_sort_cs.push_back(ID_cs);
					visited_point[ID_cs] = true;
				}
			}

		}

	int num_CS_at_topology = 1;
	for (auto iter : top_sort_cs)
	{
		cout << " " << num_CS_at_topology++ << " -> ID [" << iter << "] " << endl;
	}

}

void find_all_shortest_path(const GTN_data& gtn_struct)
{
	GTN_class gtn;
	gtn_struct >> gtn;
	dijkstra_algorithm(gtn);
}

void out_shortest_path(int initial_point, map<int, int>& copy_tmp_cs, GTN_class& gtn)
{
	string delimeter_screen = "*******************************************************************************";
	cout << delimeter_screen << endl;
	cout << " Initial point:[" << initial_point << "]\n" << endl;

	for (auto iter_copy_tmp_cs : copy_tmp_cs)
	{
		queue<int> cs_queue;
		cs_queue.push(iter_copy_tmp_cs.first);

		list<int> short_path = { iter_copy_tmp_cs.first };

		bool flag = true;
		while (!cs_queue.empty() && flag)
		{
			for (auto iter_inv_adj_matrix : gtn.inv_adjacency_matrix[cs_queue.front()])
				if (copy_tmp_cs[cs_queue.front()] - (*iter_inv_adj_matrix.second).get_lenght() == copy_tmp_cs[iter_inv_adj_matrix.first])
				{
					cs_queue.push(iter_inv_adj_matrix.first);
					short_path.push_back(iter_inv_adj_matrix.first);
				}
			cs_queue.pop();
		}

		reverse(short_path.begin(), short_path.end());

		for (auto iter_short_path : short_path)
			cout << " " << iter_short_path << " -> ";
		cout << " L: " << copy_tmp_cs[*(--short_path.end())] << endl;
	}
	cout << endl;
}

void dijkstra_algorithm(GTN_class& gtn)
{
	map<int, int> temp_cs;

	for (auto iter1_adj_matrix : gtn.adjacency_matrix)
	{
		temp_cs[iter1_adj_matrix.first] = INT_MAX;
		for (auto iter2_adj_matrix : iter1_adj_matrix.second)
			temp_cs[iter2_adj_matrix.first] = INT_MAX;
	}

	cout << "                          ALL PAIRS SHORTEST PATH " << endl;

	for (auto iter1_adj_matrix : gtn.adjacency_matrix)
	{
		queue<int> vert_queue;
		vert_queue.push(iter1_adj_matrix.first);

		map<int, int> copy_tmp_cs = temp_cs;
		copy_tmp_cs[iter1_adj_matrix.first] = 0;

		while (!vert_queue.empty())
		{
			for (auto iter2_adj_matrix : gtn.adjacency_matrix[vert_queue.front()])
			{
				copy_tmp_cs[iter2_adj_matrix.first] = min(copy_tmp_cs[vert_queue.front()] + (!iter2_adj_matrix.second->get_repair() ? iter2_adj_matrix.second->get_lenght() : INT_MAX), copy_tmp_cs[iter2_adj_matrix.first]);
				vert_queue.push(iter2_adj_matrix.first);
			}
			vert_queue.pop();
		}
		for (auto iter = copy_tmp_cs.begin(); iter != copy_tmp_cs.end(); )
			if ((iter->second == 0) || (iter->second == INT_MAX))
				iter = copy_tmp_cs.erase(iter);
			else
				++iter;
		if (copy_tmp_cs.size())
			out_shortest_path(iter1_adj_matrix.first, copy_tmp_cs, gtn);
	}
}

void find_max_flow(const GTN_data& gtn_struct)
{
	GTN_class gtn;
	gtn_struct >> gtn;
	ford_falkerson(gtn);
}

bool breadth_first_search(GTN_class& gtn, map<int, int>& parent, int Source, int Stock)
{
	queue<int> vert_queue;
	vert_queue.push(Source);

	map <int, bool> visited_vert;
	for (auto iter1_flow_matrix : gtn.flow_matrix)
	{
		visited_vert[iter1_flow_matrix.first] = false;
		for (auto iter2_flow_matrix : iter1_flow_matrix.second)
			visited_vert[iter2_flow_matrix.first] = false;
	}

	while (!vert_queue.empty())
	{
		for (auto iter : gtn.flow_matrix[vert_queue.front()])
			if (!(visited_vert[iter.first]) && (gtn.flow_matrix[vert_queue.front()][iter.first] > 0))
			{
				parent[iter.first] = vert_queue.front();
				visited_vert[iter.first] = true;
				if (iter.first == Stock)
					return true;
				vert_queue.push(iter.first);
			}

		vert_queue.pop();
	}
	return false;
}

void ford_falkerson(GTN_class& gtn)
{
	int total_total_max_flow = 0;
	for (auto iter_adj_matrix : gtn.adjacency_matrix)
		if (gtn.inv_adjacency_matrix.find(iter_adj_matrix.first) == gtn.inv_adjacency_matrix.end())
		{
			int tot_flow = 0;
			int Source = iter_adj_matrix.first;
			cout << endl << "                                   Source [" << Source << "]" << endl;

			for (auto iter_inv_adj_matrix : gtn.inv_adjacency_matrix)
				if (gtn.adjacency_matrix.find(iter_inv_adj_matrix.first) == gtn.adjacency_matrix.end())
				{
					int Stock = iter_inv_adj_matrix.first;
					cout << " Stock [" << Stock << "]" << endl;

					map<int, int> parent;
					int max_flow = 0;
					while (breadth_first_search(gtn, parent, Source, Stock))
					{
						unordered_set<int> way_cs;
						int path_flow = INT_MAX;
						for (int one = Stock; one != Source; one = parent[one])
						{
							int two = parent[one];
							way_cs.insert(one);
							way_cs.insert(two);
							path_flow = min(path_flow, gtn.flow_matrix[two][one]);
						}

						list <int> way_cs_reverse;
						for (auto iter_way_cs : way_cs)
							way_cs_reverse.push_front(iter_way_cs);
						cout << " WAY: ";
						for (auto iter_way_cs_reverse : way_cs_reverse)
							cout << " ->" << iter_way_cs_reverse;

						for (int one = Stock; one != Source; one = parent[one])
						{
							int two = parent[one];
							gtn.flow_matrix[two][one] -= path_flow;
						}
						max_flow += path_flow;
						cout << endl;

					}
					//if (max_flow) cout << " Found max flow: " << max_flow << endl;
					tot_flow += max_flow;
					cout << " Found max flow: " << max_flow << endl << endl;
				}
			total_total_max_flow += tot_flow;
			cout << "\n________________________" << " Total max flow: " << tot_flow << endl;
		}
	cout << " Summary all gas transmission network max flow:  " << total_total_max_flow << endl;

	/*for (auto iter_cs_source : gtn.adjacency_matrix)
	{
		int Source = iter_cs_source.first;
		for (auto iter_cs_stock : gtn.inv_adjacency_matrix)
		{
			int Stock = iter_cs_stock.first;

			map<int, int> parent;
			int max_flow = 0;
			while (breadth_first_search(gtn, parent, Source, Stock))
			{
				int path_flow = INT_MAX;
				for (int one = Stock; one != Source; one = parent[one])
				{
					int two = parent[one];
					cout << "  ->" << one << " -> " << two;
					path_flow = min(path_flow, gtn.flow_matrix[two][one]);
				}

				for (int one = Stock; one != Source; one = parent[one])
				{
					int two = parent[one];
					gtn.flow_matrix[two][one] -= path_flow;
				}
				max_flow += path_flow;
			}
			if ( max_flow ) cout << " Found max flow: " << max_flow << endl;
		}
	}*/
}
