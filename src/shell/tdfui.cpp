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

TDFUI::TDFUI(int argc, char * argv[]) {

	//cmd["graph"] = bind(&TDFUI::display_rdf_graph, this);
	cmd["run"] = bind(&TDFUI::run_graph, this);
	cmd["h"] = bind(&TDFUI::display_help, this);

	//comment["graph"] = "\tdisplay rdf data graph.";
	comment["run"] = "\trun the data graph.";
	comment["h"] = "\tdisplay help menu.";

	if (argc != 2) {
		cout << "usage: tdf <file.df>\n";
		exit(0);
	}
	else {	
		load_from_file(argv[1]);
	}
}

int TDFUI::load_from_file(const char * filename) {
	int ret;
	graph = new Graph();
	string rdfname;
	ifstream file(filename);
        read_str(file, "df");
        file >> rdfname;
        graph->set_name(rdfname);
	ret = read_graph(file, graph);
	if (ret < 0)
		return ret;
	if (ret == 0) {
		cout << "tdf is loaded successfully.\n";
		display_help();
		return 0;
	} else
		return ret;
}

int TDFUI::read_str(ifstream & file, string str) {
	string tmp;
	file >> tmp;
	if (tmp == str)
		return 0;
	else if (file.eof()) {
		return -2;
	}
	else {
		cout << "error: cannot read " << str << "\n";
		return -1;
	}
}

int TDFUI::read_graph(ifstream & file, Graph * g) {
	int ret;
	string tmp;
	read_str(file, "{");
	read_str(file, "topology");
	ret = read_topology(file,g);
	if (ret < 0)
		return ret;
        while (true) {
		file >> tmp;
		if (tmp == "}")
			break;
		else if (tmp == "actor")
			read_props(file, g);
		else if (tmp == "production")
			read_productions(file, g);
		else if (tmp == "consumption")
			read_consumptions(file, g);
		else {
			cout << "cannot parse the graph.\n";
			return -1;
		}
	}
	load_actor_types(g);
	return 0;
}

int TDFUI::load_actor_types(Graph * g) {

	string type;
	auto actors = g->get_actors();
	for (auto ac : actors) {
		type = g->get_actor_prop(ac, "computation");
		g->set_actor_type(ac,type);
	}

	return 0;
}


int TDFUI::read_productions(ifstream & file, Graph * g) {

	int ret;
	string ratelist, edgename, rate;
	read_str(file, "{");	
	getline(file, ratelist, '}');

	auto end_pos = std::remove(ratelist.begin(), ratelist.end(), ' ');
	ratelist.erase(end_pos, ratelist.end());

	end_pos = std::remove(ratelist.begin(), ratelist.end(), '\n');
	ratelist.erase(end_pos, ratelist.end());

	end_pos = std::remove(ratelist.begin(), ratelist.end(), '\t');
	ratelist.erase(end_pos, ratelist.end());

	istringstream ss(ratelist);
	while (getline(ss, rate, ';')) {
		ret = add_production_rate(rate,g);
		if (ret < 0)
			return ret;		
	}
	return 0;
}

int TDFUI::read_consumptions(ifstream & file, Graph * g) {

	int ret;
	string ratelist, edgename, rate;
	read_str(file, "{");	
	getline(file, ratelist, '}');

	auto end_pos = std::remove(ratelist.begin(), ratelist.end(), ' ');
	ratelist.erase(end_pos, ratelist.end());

	end_pos = std::remove(ratelist.begin(), ratelist.end(), '\n');
	ratelist.erase(end_pos, ratelist.end());

	end_pos = std::remove(ratelist.begin(), ratelist.end(), '\t');
	ratelist.erase(end_pos, ratelist.end());

	istringstream ss(ratelist);
	while (getline(ss, rate, ';')) {
		ret = add_consumption_rate(rate,g);
		if (ret < 0)
			return ret;		
	}
	return 0;
}

int TDFUI::read_props(ifstream & file, Graph * g) {

	int ret;
	string actname, props, prop;
	file >> actname;
	read_str(file, "{");	
	getline(file, props, '}');

	auto end_pos = std::remove(props.begin(), props.end(), ' ');
	props.erase(end_pos, props.end());

	end_pos = std::remove(props.begin(), props.end(), '\n');
	props.erase(end_pos, props.end());

	end_pos = std::remove(props.begin(), props.end(), '\t');
	props.erase(end_pos, props.end());

	istringstream ss(props);
	while (getline(ss, prop, ';')) {
		ret = add_prop(actname, prop,g);
		if (ret < 0)
			return ret;		
	}
	return 0;

}


int TDFUI::read_topology(ifstream & file, Graph * g) {
        int ret;
	string actorlist, edgelist, actor, edge;
	read_str(file, "{");
	read_str(file, "nodes");
	read_str(file, "=");
	getline(file, actorlist, ';');

	read_str(file, "edges");
	read_str(file, "=");
	getline(file, edgelist, ';');

	read_str(file, "}");
	
	auto end_pos = std::remove(actorlist.begin(), actorlist.end(), ' ');
	actorlist.erase(end_pos, actorlist.end());

	end_pos = std::remove(actorlist.begin(), actorlist.end(), '\n');
	actorlist.erase(end_pos, actorlist.end());

	end_pos = std::remove(actorlist.begin(), actorlist.end(), '\t');
	actorlist.erase(end_pos, actorlist.end());

	end_pos = std::remove(edgelist.begin(), edgelist.end(), ' ');
	edgelist.erase(end_pos, edgelist.end());

	end_pos = std::remove(edgelist.begin(), edgelist.end(), '\n');
	edgelist.erase(end_pos, edgelist.end());

	end_pos = std::remove(edgelist.begin(), edgelist.end(), '\t');
	edgelist.erase(end_pos, edgelist.end());

	istringstream ssa(actorlist);
	while (getline(ssa, actor, ',')) {
		ret = add_actor(actor,g);
		if (ret < 0)
			return ret;		
	}

	istringstream sse(edgelist);
	while (getline(sse, edge, ')')) {
		if (edge[0]==',') 
			edge = edge.substr(1, edge.size());
		ret = add_edge(edge,g);
		if (ret < 0)
			return ret;		
	}

	return 0;
}

int TDFUI::add_edge(const string& edge, Graph * g) {
        string edge_name, edge_source, edge_sink;

	if (edge=="") {
		cout << "error: edge is not well formatted.\n";
		return -2;
	}
	std::istringstream ss(edge);
  	getline(ss, edge_name, '(');
  	getline(ss, edge_source, ',');
  	getline(ss, edge_sink);

	int ret = g->add_edge(edge_name, edge_source, edge_sink);
	if (ret == -2)
		cout << "error: edge already exists.\n";
	else if (ret == -1)
		cout << "error: edge source/sink actor does not exist.\n";
	return ret;
}

int TDFUI::add_actor(const string& actor, Graph * g) {
        if (actor=="") {
		cout << "error: actor is not well formatted.\n";
		return -2;
	}
	int ret = g->add_actor(actor, "");
	return ret;
}

int TDFUI::read_actors(ifstream & file, Graph * g) {
        int ret;
	string actorlist, actor;
	read_str(file, "{");
	getline(file, actorlist, '}');

	auto end_pos = std::remove(actorlist.begin(), actorlist.end(), ' ');
	actorlist.erase(end_pos, actorlist.end());

	end_pos = std::remove(actorlist.begin(), actorlist.end(), '\n');
	actorlist.erase(end_pos, actorlist.end());

	end_pos = std::remove(actorlist.begin(), actorlist.end(), '\t');
	actorlist.erase(end_pos, actorlist.end());


	istringstream ss(actorlist);
	while (getline(ss, actor, ';')) {
		ret = add_actortype(actor,g);
		if (ret < 0)
			return ret;		
	}
	return 0;
}

int TDFUI::read_edges(ifstream & file, Graph * g) {
	int ret;	
	int source, sink;
	string edgelist, edgeline, source_str, sink_str;
	string edge, edge_source, edge_sink;
	read_str(file, "{");

	getline(file, edgelist, '}');

	auto end_pos = std::remove(edgelist.begin(), edgelist.end(), ' ');
	edgelist.erase(end_pos, edgelist.end());

	end_pos = std::remove(edgelist.begin(), edgelist.end(), '\n');
	edgelist.erase(end_pos, edgelist.end());

	end_pos = std::remove(edgelist.begin(), edgelist.end(), '\t');
	edgelist.erase(end_pos, edgelist.end());

	istringstream ss(edgelist);

	while (getline(ss, edgeline, ';')) {
		istringstream edl(edgeline);
		getline(edl, edge_source, ',');
		getline(edl, edge_sink, ',');
		getline(edl, source_str, ',');
		getline(edl, sink_str);
		ret = add_edge(edge_source, edge_sink, g);
		if (ret < 0) {
			cout << "error: parsing file failed.\n";
			return ret;
		}
		source = std::stoi(source_str);
		sink = std::stoi(sink_str);

		if (source < 1 || sink < 1) {
			cout << "error: source/sink rates must be valid positive integer.\n";
			return ret;
		}
		edge = edge_source + edge_sink;
		g->set_source_rate(edge, source);
		g->set_sink_rate(edge, sink);
	}
	return 0;
}

int TDFUI::add_production_rate(const string& rate, Graph * g) {
	int edr=1;
	string edgename="", edgerate="";
	std::istringstream ss(rate);
  	getline(ss, edgename, '=');
	getline(ss, edgerate);
        if (edgename=="" || edgerate=="") {
		cout << "error: edge production rate is not well formatted.\n";
		return -2;
	}
	try {
		edr = stoi(edgerate);
	}
	catch(...) {
		cout << "error: production rate of edge " << edgename << " is not correct.\n";
		return -2;
	}
	int ret = g->set_source_rate(edgename,edr);
	if (ret == -1)
		cout << "error: edge " << edgename << " is not found.\n";
	return ret;
}

int TDFUI::add_consumption_rate(const string& rate, Graph * g) {
	int edr=1;
	string edgename="", edgerate="";
	std::istringstream ss(rate);
  	getline(ss, edgename, '=');
	getline(ss, edgerate);
        if (edgename=="" || edgerate=="") {
		cout << "error: edge consumption rate is not well formatted.\n";
		return -2;
	}
	try {
		edr = stoi(edgerate);
	}
	catch(...) {
		cout << "error: consumption rate of edge " << edgename << " is not correct.\n";
		return -2;
	}
	int ret = g->set_sink_rate(edgename,edr);
	if (ret == -1)
		cout << "error: edge " << edgename << " is not found.\n";
	return ret;
}

int TDFUI::add_prop(const string& actname, const string& prop, Graph * g) {
	string key="", val="";
	std::istringstream ss(prop);
  	getline(ss, key, '=');
	getline(ss, val);
        if (key=="" || val=="") {
		cout << "error: actor property is not well formatted.\n";
		return -2;
	}
	int ret = g->add_prop(actname, key, val);
	return ret;
}

int TDFUI::add_actortype(const string& actortype, Graph * g) {
	string name="", type="";
	std::istringstream ss(actortype);
  	getline(ss, name, ':');
	getline(ss, type);
        if (name=="" || type=="") {
		cout << "error: actor is not well formatted.\n";
		return -2;
	}
	int ret = g->add_actor(name, type);
	return ret;
}

int TDFUI::add_edge(string edge_source, string edge_sink, Graph * g) {
	int ret = g->add_edge(edge_source, edge_sink);
	if (ret == -2)
		cout << "error: edge already exists.\n";
	else if (ret == -1)
		cout << "error: edge source/sink actor does not exist.\n";
	//else if (ret == 0) {
	//	cout << edge_source << " -> " << edge_sink << "\n";
	//}
	return ret;
}

int TDFUI::display_rdf_graph() {
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

int TDFUI::display_graph(Graph * g) {
	int ac_index = 0;
	int ed_index = 0;
	vector<string> actorlist = g->get_actors();
	vector<string> edgelist = g->get_edges();
	//vector<vector<string>> pathlist = g->path();
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
	/*
	cout << "-------\n";
	cout << "paths \n\n";
	for (auto & path : pathlist) {
		for (int j=0; j<path.size()-1; j++) {
			cout << path[j] << " -> "; 
		}
		cout << path[path.size()-1] << "\n";
	}
	*/
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
	//int ret = graph->solve2();
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

	map<string, df::Actor *> actormap;

	df::Dataflow* dataflow = df::Factory::createDataflow(graph->get_name());
	
	vector<string> actorlist = graph->get_actors();
	for (auto & acname : actorlist) {
		string actype = graph->get_actor_type(acname);
		if (actype == "") {
			cout << "error: actor type cannot be unknown.\n";
			cout << "set the property computation of actor " << acname << "\n";
			return -1;
		}
		df::Actor * actor = df::Factory::createActor(actype, acname);
		dataflow->addActors(actor, nullptr);
		map<string, string> props = graph->get_actor_props(acname);
		for (auto p : props) {
			actor->setProp(p.first, p.second);
		}
		actormap[acname] = actor;
	}

	vector<string> edgelist = graph->get_edges();
	for (auto & ed : edgelist) {
		df::Actor * src = actormap[graph->get_source_name(ed)];
		df::Actor * snk = actormap[graph->get_sink_name(ed)];
  		dataflow->connectActors(src, snk, graph->get_source_rate(ed), graph->get_sink_rate(ed));
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

