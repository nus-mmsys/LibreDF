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

#include "parser.h"

Parser::Parser() {
}

int Parser::load_from_file(const char * filename) {
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
		return 0;
	} else
		return ret;
}

int Parser::read_str(ifstream & file, string str) {
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

int Parser::trim_str(string & str) {

	auto end_pos = std::remove(str.begin(), str.end(), ' ');
	str.erase(end_pos, str.end());

	end_pos = std::remove(str.begin(), str.end(), '\n');
	str.erase(end_pos, str.end());

	end_pos = std::remove(str.begin(), str.end(), '\t');
	str.erase(end_pos, str.end());

	return 0;
}
int Parser::read_graph(ifstream & file, Graph * g) {
	int ret;
	string tmp;
	read_str(file, "{");
        read_str(file, "topology");
	ret = read_topology(file,g);
	if (ret < 0)
		return ret;
	while (true) {
		ret = 0;
		file >> tmp;
		if (tmp == "}")
			break;
		else if (tmp == "parameter")
			ret = read_parameters(file, g);
		else if (tmp == "actor")
			ret = read_props(file, g);
		else if (tmp == "production")
			ret = read_productions(file, g);
		else if (tmp == "consumption")
			ret = read_consumptions(file, g);
		else {
			cout << "cannot parse the graph.\n";
			return -1;
		}
		if (ret < 0)
			return ret;
	}
	load_actor_types(g);
	return 0;
}

int Parser::load_actor_types(Graph * g) {

	string type;
	auto actors = g->get_actors();
	for (auto ac : actors) {
		type = g->get_actor_prop(ac, "computation");
		g->set_actor_type(ac,type);
	}

	return 0;
}


int Parser::read_productions(ifstream & file, Graph * g) {

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

int Parser::read_consumptions(ifstream & file, Graph * g) {

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

int Parser::read_parameters(ifstream & file, Graph * g) {

	int ret;
	string params;
	read_str(file, "{");	
	getline(file, params, '}');

	return 0;
}

int Parser::read_props(ifstream & file, Graph * g) {

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

int Parser::read_topology(ifstream & file, Graph * g) {
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

	trim_str(actorlist);
	trim_str(edgelist);

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

int Parser::add_edge(const string& edge, Graph * g) {
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

int Parser::add_actor(const string& actor, Graph * g) {
        if (actor=="") {
		cout << "error: actor is not well formatted.\n";
		return -2;
	}
	int ret = g->add_actor(actor, "");
	return ret;
}

int Parser::read_actors(ifstream & file, Graph * g) {
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

int Parser::read_edges(ifstream & file, Graph * g) {
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

int Parser::add_production_rate(const string& rate, Graph * g) {
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

int Parser::add_consumption_rate(const string& rate, Graph * g) {
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

int Parser::add_prop(const string& actname, const string& prop, Graph * g) {
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

int Parser::add_actortype(const string& actortype, Graph * g) {
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

int Parser::add_edge(string edge_source, string edge_sink, Graph * g) {
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

Graph * Parser::get_graph() {
	return graph;
}
