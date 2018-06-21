/*
 *   TDF
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

#include "tdfui.h"

TDFUI::TDFUI(int argc, char * argv[], Parser * p) {

	//cmd["graph"] = bind(&TDFUI::display_rdf_graph, this);
	cmd["run"] = bind(&TDFUI::run_graph, this);
	cmd["h"] = bind(&TDFUI::display_help, this);

	//comment["graph"] = "\tdisplay rdf data graph.";
	comment["run"] = "\trun the data graph.";
	comment["h"] = "\tdisplay help menu.";

	parser = p;
        
	if (argc != 2) {
		cout << "usage: tdf <file.df>\n";
		exit(0);
	}
	else {	
		parser->load_from_file(argv[1]);
		display_help();
	}
}

int TDFUI::display_df_graph() {
	int ret;
	Graph * g = parser->get_graph();
	cout << "=======\n";
	cout << g->get_name() << "\n";
	cout << "=======\n";
	ret = display_graph(g);
	if (ret < 0)
		return ret;
	cout << "=======\n";
	return 0;
}

int TDFUI::display_graph(Graph * g) {
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
	cout << "topological orders \n\n";
	for (auto & order : orderlist) {
		for (int j=0; j<order.size(); j++) {
			cout << order[j] << " "; 
		}
		cout << "\n";
	}
	cout << "-------\n";
	cout << "single appearance schedules \n\n";
	for (auto & sas : saslist) {
		cout << sas << "\n"; 
	}
	cout << "-------\n";

	cout << "solutions \n\n";
	int ret = g->solve();
	if (ret == -1)
		cout << "error: graph is not consistent.\n";
	else {
		for (auto & ac : actorlist) {
			cout << ac << " : " << g->get_firing(ac) << "\n";
		}
	}
	
	cout << "-------\n";
	return 0;
}

int TDFUI::display_help() {
	cout << "press q to exit.\n";
	cout << "commands:\n";
	for (auto & command : cmd)
		cout << "\t" << command.first << "\t" << comment[command.first] << "\n";
	return 0;
}

int TDFUI::run_graph() {

	Graph * g = parser->get_graph();
	map<string, df::Actor *> actormap;

	df::Dataflow* dataflow = df::Factory::createDataflow(g->get_name());
	
	vector<string> actorlist = g->get_actors();
	for (auto & acname : actorlist) {
		string actype = g->get_actor_type(acname);
		if (actype == "") {
			cout << "error: actor type cannot be unknown.\n";
			cout << "set the property computation of actor " << acname << "\n";
			return -1;
		}
		df::Actor * actor = df::Factory::createActor(actype, acname);
		dataflow->addActors(actor, nullptr);
		map<string, string> props = g->get_actor_props(acname);
		for (auto p : props) {
			actor->setProp(p.first, p.second);
		}
		actormap[acname] = actor;
	}

	vector<string> edgelist = g->get_edges();
	for (auto & ed : edgelist) {
		df::Actor * src = actormap[g->get_source_name(ed)];
		df::Actor * snk = actormap[g->get_sink_name(ed)];
  		dataflow->connectActors(src, snk, g->get_source_rate(ed), g->get_sink_rate(ed));
	}
 
	dataflow->init();

  	dataflow->run();

	df::Factory::destroyDataflow(dataflow);
	
	return 0;
}
int TDFUI::process_command(string command) {

	if (command == "q")
		return -1;

	if (cmd.find(command) != cmd.end())
		cmd[command]();
	else
		cout << "command not found. press h for help.\n";

	return 0;
}

int TDFUI::loop() {

	int ret = 0;	
	string command = "";
	do {
		cout << ">";
		cin >> command;
		ret = process_command(command);
	} while (ret >= 0);
	return ret;
}
