#include "gtn.h"

GTN_class& operator>>(const GTN_data& gtn_struct, GTN_class& gtn)
{
	for (auto iter_connect_cs : gtn_struct.connect_struct) {
		if (!(
			gtn_struct.cs_struct.mp_cs.count(iter_connect_cs.initial_cs_id) && gtn_struct.cs_struct.mp_cs.count(iter_connect_cs.terminal_cs_id)
			&& gtn_struct.pipe_struct.mp_pipe.count((iter_connect_cs.pipeline_id)->get_id_pipeline())
			&& !(*iter_connect_cs.pipeline_id).get_repair() && (*iter_connect_cs.pipeline_id).get_used()
			)) continue;
		gtn.adjacency_matrix[iter_connect_cs.initial_cs_id][iter_connect_cs.terminal_cs_id] = iter_connect_cs.pipeline_id;
		gtn.inv_adjacency_matrix[iter_connect_cs.terminal_cs_id][iter_connect_cs.initial_cs_id] = iter_connect_cs.pipeline_id;
		gtn.flow_matrix[iter_connect_cs.initial_cs_id][iter_connect_cs.terminal_cs_id] = (*iter_connect_cs.pipeline_id).get_diameter() * (*iter_connect_cs.pipeline_id).get_lenght() / 100;
	}

	return(gtn);
}

// out screen
ostream& operator<<(ostream& os, const GTN_class& some_network)
{
	cout << "\n [CS ID] -> (CS ID), P: Pipeline ID, L: Lenght, D: Diameter, F: Flow \n" << endl;
	for (auto& iter_some_network : some_network.adjacency_matrix)
	{
		os << " [" << iter_some_network.first << "] " << ">";
		for (auto& iter_term : some_network.adjacency_matrix.at(iter_some_network.first))
			os << " (" << iter_term.first << ") {" << "P:" << (*iter_term.second).get_id_pipeline() << ",L:" << (*iter_term.second).get_lenght() << ",D:" << (*iter_term.second).get_diameter() <<
			",F:" << (*iter_term.second).get_diameter() * (*iter_term.second).get_lenght() / 100 << "}";
		os << endl;
	}

	return os;
}

// in file
ifstream& operator>>(ifstream& ifs, GTN_data& gtn_struct)
{
	int value;
	CS_connected new_connect;
	ifs >> value;
	new_connect.initial_cs_id = gtn_struct.cs_struct.mp_cs.at(value).get_cs_id();
	ifs >> value;
	new_connect.terminal_cs_id = gtn_struct.cs_struct.mp_cs.at(value).get_cs_id();
	ifs >> value;
	gtn_struct.pipe_struct.mp_pipe.at(value).set_used(true);
	new_connect.pipeline_id = &gtn_struct.pipe_struct.mp_pipe.at(value);
	gtn_struct.connect_struct.push_back(new_connect);
	return ifs;
};

// out file
ofstream& operator<<(ofstream& ofs, const vector<CS_connected>& connect_struct)
{
	ofs << 3 << endl;
	for (auto& iter_connect_cs : connect_struct)
		ofs << iter_connect_cs.initial_cs_id << " " << iter_connect_cs.terminal_cs_id << " " << (*iter_connect_cs.pipeline_id).get_id_pipeline() << endl;
	return ofs;
}