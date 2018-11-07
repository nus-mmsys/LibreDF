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

#include "analyse.h"

Analyse::Analyse(int argc, char * argv[], Parser * p) {

	if (argc != 2) {
		cout << "usage: " << argv[0] << " <file.tmf>\n";
		exit(0);
	}

	cmd["graph"] = bind(&Analyse::display_df_graph, this);
	cmd["run"] = bind(&Analyse::run, this, false);
	cmd["runtcp"] = bind(&Analyse::run, this, true);
	cmd["h"] = bind(&Analyse::display_help, this);
	cmd["latency"] = bind(&Analyse::latency, this);
	cmd["schedule"] = bind(&Analyse::schedule, this);
	
	comment["graph"] = "\tdisplay the graph.";
	comment["run"] = "\trun the graph on shared memory.";
	comment["runtcp"] = "\trun the graph on tcp.";
	comment["h"] = "\tdisplay help menu.";
	comment["latency"] = "\tcompute the lateny of the graph.";
	comment["schedule"] = "compute an schedule for the graph.";

	parser = p;	
	parser->load_from_file(argv[1]);
	graph = parser->get_graph();
}

int Analyse::display_df_graph() {
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

int Analyse::display_graph(Graph * g) {
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

int Analyse::display_help() {
	cout << "press q to exit.\n";
	cout << "commands:\n";
	for (auto & command : cmd)
		cout << "\t" << command.first << "\t" << comment[command.first] << "\n";
	return 0;
}

int Analyse::process_command(string command) {

	if (command == "q")
		return -1;

	if (cmd.find(command) != cmd.end())
		cmd[command]();
	else
		cout << "command not found. press h for help.\n";

	return 0;
}

int Analyse::loop() {

	int ret = 0;	
	string command = "";
	do {
		cout << ">";
		cin >> command;
		ret = process_command(command);
	} while (ret >= 0);
	return ret;
}

int Analyse::run(bool dist) {

	df::Dataflow * dfg = parser->get_dataflow();

	std::string localhost = "127.0.0.1";
		
	dfg->replaceProp<bool>("distributed", dist);
	dfg->replaceProp("discovery_host", localhost);
	dfg->replaceActorsProp("host", localhost);

	dfg->init();

	dfg->runDiscovery();

	dfg->connect();

  	dfg->run();

	dfg->waitDiscovery();

	delete dfg;

	return 0;
}

int Analyse::latency() {

	int latency = 0;
    	std::chrono::high_resolution_clock::time_point start, end; 			
	start = std::chrono::high_resolution_clock::now();
	latency = graph->latency();
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Latency = " << latency << "\n";

	std::cout << "Execution time = " << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()  << " us\n"; 
 
	return 0;
}

int Analyse::schedule() {

    	std::chrono::high_resolution_clock::time_point start, end; 			
	start = std::chrono::high_resolution_clock::now();
	map<string, vector<tuple<int,int>>> sch = graph->schedule();
	end = std::chrono::high_resolution_clock::now();

	for (auto ac : sch) {
		cout << ac.first << ": ";
		for (auto t : ac.second) {
			cout << "[" << get<0>(t) << " " << get<1>(t) << "] ";
		}
		cout << "\n";
	}		
	std::cout << "Execution time = " << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()  << " us\n"; 
 
	return 0;
}

