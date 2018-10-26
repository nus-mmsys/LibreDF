/*
 *   TMF
 *   Copyright (C) TMF Team
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "graph_computation.h"

GraphComputation::GraphComputation(int argc, char * argv[], Parser * p) {

	if (argc != 2) {
		cout << "usage: " << argv[0] << " <file.tmf>\n";
		exit(0);
	}

	cmd["graph"] = bind(&GraphComputation::display_df_graph, this);
	cmd["hamilton"] = bind(&GraphComputation::hamilton, this);
	cmd["adjacency"] = bind(&GraphComputation::adjacency, this);
	cmd["mxpower"] = bind(&GraphComputation::mxpower, this);
	cmd["latency"] = bind(&GraphComputation::latency, this);
	cmd["h"] = bind(&GraphComputation::display_help, this);

	comment["graph"] = "\t\tdisplay the graph.";
	comment["hamilton"] = "\tcompute the hamiltonian paths.";
	comment["adjacency"] = "\tdisplay the adjacency matrix.";
	comment["mxpower"] = "\t\tcompute the adjacency matrix to the power of N in Max-Plus algebra.";
	comment["latency"] = "\t\tcompute the lateny of the graph.";
	comment["h"] = "\t\tdisplay help menu.";

	parser = p;	
	parser->load_from_file(argv[1]);
	graph = parser->get_graph();
}

int GraphComputation::display_df_graph() {
	int ret;
	cout << "=======\n";
	cout << graph->get_name() << "\n";
	cout << "=======\n";
	ret = display_graph(graph);
	if (ret < 0)
		return ret;
	cout << "=======\n";
	return 0;
}

int GraphComputation::display_graph(Graph * g) {
	int ac_index = 0;
	int ed_index = 0;
	vector<string> actorlist = g->get_actors();
	vector<string> edgelist = g->get_edges();
	vector<string> saslist = g->sas();
	vector<vector<string>> orderlist = g->order();
	cout << "-------\n";
	cout << "actors \n\n";
	for (auto & ac : actorlist) {
		cout << to_string(ac_index) << ": " 
			<< ac << " : " 
		       	<< g->get_actor_type(ac) << "\n";
			ac_index++;
	}
	cout << "-------\n";
	cout << "edges \n\n";
	for (auto & ed : edgelist) {
		cout << to_string(ed_index) << ": " 
			<< g->get_source_name(ed) << " (" 
			<< g->get_source_rate(ed)  << ") -> (" 
			<< g->get_sink_rate(ed) << ") "
		       	<< g->get_sink_name(ed) << "\n";
			ed_index++;
	}
	cout << "-------\n";
	return 0;
}

int GraphComputation::display_help() {
	cout << "press q to exit.\n";
	cout << "commands:\n";
	for (auto & command : cmd)
		cout << "\t" << command.first << "\t" << comment[command.first] << "\n";
	return 0;
}

int GraphComputation::process_command(string command) {

	if (command == "q")
		return -1;

	if (cmd.find(command) != cmd.end())
		cmd[command]();
	else
		cout << "command not found. press h for help.\n";

	return 0;
}

int GraphComputation::loop() {

	int ret = 0;	
	string command = "";
	do {
		cout << ">";
		cin >> command;
		ret = process_command(command);
	} while (ret >= 0);
	return ret;
}

int GraphComputation::hamilton() {

    	std::chrono::high_resolution_clock::time_point start, end; 			
	start = std::chrono::high_resolution_clock::now();
	vector<vector<string>> paths = graph->hamiltonians();
	end = std::chrono::high_resolution_clock::now();
	for (auto path : paths) {
		for (auto actor : path)
			cout << actor << " ";
		cout << "\n";
	}
	
	std::cout << "Execution time = " << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()  << " us\n"; 
 
	return 0;
}

int GraphComputation::adjacency() {

    	std::chrono::high_resolution_clock::time_point start, end; 			
	start = std::chrono::high_resolution_clock::now();
	vector<vector<int>> adj = graph->adjacency();
	end = std::chrono::high_resolution_clock::now();
	for (auto row : adj) {
		for (auto col : row)
			cout << col << " ";
		cout << "\n";
	}
	
	std::cout << "Execution time = " << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()  << " us\n"; 
 
	return 0;
}

int GraphComputation::mxpower() {

	int n;
	cout << "mxpower> enter N: ";
	cin >> n;
	cout << "\n";
    	std::chrono::high_resolution_clock::time_point start, end; 			
	start = std::chrono::high_resolution_clock::now();
	vector<vector<int>> pow = graph->mxpower(n);
	end = std::chrono::high_resolution_clock::now();
	for (auto row : pow) {
		for (auto col : row)
			cout << col << " ";
		cout << "\n";
	}
	
	std::cout << "Execution time = " << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()  << " us\n"; 
 
	return 0;
}

int GraphComputation::latency() {

    	std::chrono::high_resolution_clock::time_point start, end; 			
	start = std::chrono::high_resolution_clock::now();
	//TODO
	end = std::chrono::high_resolution_clock::now();
	
	std::cout << "Execution time = " << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()  << " us\n"; 
 
	return 0;
}

