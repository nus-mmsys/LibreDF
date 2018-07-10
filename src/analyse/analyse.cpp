/*
 *   TMF
 *   Copyright (C) 2018
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
		cout << "usage: " << argv[0] << " <file.df>\n";
		exit(0);
	}

	cmd["graph"] = bind(&Analyse::display_df_graph, this);
	cmd["run"] = bind(&Analyse::run, this, false);
	cmd["runtcp"] = bind(&Analyse::run, this, true);
	cmd["h"] = bind(&Analyse::display_help, this);

	comment["graph"] = "\tdisplay the graph.";
	comment["run"] = "\trun the graph on shared memory.";
	comment["runtcp"] = "\trun the graph on tcp.";
	comment["h"] = "\tdisplay help menu.";
		
	p->load_from_file(argv[1]);
	graph = p->get_graph();
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

	map<string, df::Actor *> actormap;
	std::string localhost = "127.0.0.1";

	//Create dataflow
	df::Dataflow dataflow(graph->get_name());
	
	map<string, string> params = graph->get_graph_params();
	for (auto p : params) {
		dataflow.setProp(p.first, p.second);
	}
		
	dataflow.replaceProp<bool>("distributed", dist);
	dataflow.replaceProp("discovery_host", localhost);
	
	//Create actors
	vector<string> actorlist = graph->get_actors();
	for (auto & acname : actorlist) {
		string actype = graph->get_actor_type(acname);
		if (actype == "") {
			cout << "error: actor type cannot be unknown.\n";
			cout << "set the property computation of actor " << acname << "\n";
			return -1;
		}
		df::Actor * actor = dataflow.createActor(actype, acname);
		map<string, string> props = graph->get_actor_props(acname);
		for (auto p : props) {
			actor->setProp(p.first, p.second);
		}
		actor->replaceProp("host", localhost);
		actormap[acname] = actor;
	}

	//Initialize dataflow
	dataflow.init();

	dataflow.runDiscovery();

	//Connect actors
	vector<string> edgelist = graph->get_edges();
	for (auto & ed : edgelist) {
		df::Actor * src = actormap[graph->get_source_name(ed)];
		df::Actor * snk = actormap[graph->get_sink_name(ed)];
		dataflow.connectActors(src, snk, ed, graph->get_source_rate(ed), graph->get_sink_rate(ed));
	}

	//Run dataflow
  	dataflow.run();

	dataflow.stopDiscovery();

	//Destructor of dataflow is called.
	
	return 0;
}
