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

#include "parser.h"

Parser::Parser() {
	dfstr = "";
	topology = "";
	parameter = "";

}

int Parser::load_from_string(const std::string& app) {
	dfstr = app;
	std::stringstream ss(app);
	return load_from_stream(ss);	
}

int Parser::load_from_file(const char * filename) {
	ifstream file(filename);
	if (!file)
       		return -1;

	std::stringstream ss;
	ss << file.rdbuf();
	file.close();
	dfstr = ss.str();
	return load_from_stream(ss);
}


int Parser::load_from_stream(stringstream& ss) {
	int ret;
	graph = new Graph();
	string gname;
        read_str(ss, "tmf");
        ss >> gname;
	graph->set_name(gname);
	ret = read_graph(ss, graph);
	if (ret < 0)
		return ret;
	if (ret == 0) {
		cout << "graph is loaded successfully.\n";
		return 0;
	} else
		return ret;
}

std::string Parser::df_all() {
	return dfstr;
}

std::string Parser::df_byip(const std::string& ip) {
	return "tmf " + graph->get_name() + " {\n" + topology + dfactor_byip(ip) + parameter + "\n}";
}

int Parser::read_str(stringstream & stream, string str) {
	string tmp;
	stream >> tmp;
	if (tmp == str)
		return 0;
	else if (stream.eof()) {
		return -2;
	}
	else {
		cout << "error: cannot read " << str << "\n";
		return -1;
	}
}

int Parser::check_str(stringstream & stream, string str) {
	string tmp;
	stream >> tmp;
	if (tmp == str)
		return 0;
	else {
		for(int i=0; i<tmp.size(); i++)
			stream.unget();
	}
	return -1;
}


void Parser::insert_lookup(std::string ip, std::string actname) {
	if (iplookup.find(ip) == iplookup.end()) {
		std::vector<std::string> v;
		v.push_back(actname);
		iplookup.insert(std::make_pair(ip, v));
	}
	else {
		iplookup[ip].push_back(actname);
	}
}

std::vector<std::string> Parser::get_ips() {
	std::vector<std::string> res;
	for (auto&& ip : iplookup)
		res.push_back(ip.first);
	return res;
}


std::string Parser::dfactor_byip(std::string ip) {
	std::string res = "";
	std::vector<std::string> actlist = iplookup[ip];
	for (auto&& ac : actlist) {
		res = res + "actor " + ac + " " + dfactor[ac] + "\n";	
	}
	return res;
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

int Parser::read_graph(stringstream & stream, Graph * g) {
	int ret;
	string tmp;
	read_str(stream, "{");
        read_str(stream, "topology");
	ret = read_topology(stream,g);
	if (ret < 0)
		return ret;
	while (true) {
		ret = 0;
		stream >> tmp;
		if (tmp == "}")
			break;
		else if (tmp == "parameter")
			ret = read_parameters(stream, g);
		else if (tmp == "actor")
			ret = read_props(stream, g);
		else if (tmp == "production")
			ret = read_productions(stream, g);
		else if (tmp == "consumption")
			ret = read_consumptions(stream, g);
		else if (tmp == "tokens")
			ret = read_tokens(stream, g);
		else {
			cout << "cannot parse the graph.\n";
			return -1;
		}
		if (ret < 0)
			return ret;
	}
	load_actor_types(g);
	load_edge_ports(g);
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

int Parser::load_edge_ports(Graph * g) {

	for (auto i = g->edge_begin(); i != g->edge_end(); i++) {
		Edge * e = i->second;
		string src = e->get_source_actor()->get_name();
		string snk = e->get_sink_actor()->get_name();

		e->set_source_port(
			g->get_actor_prop_key(src, i->first));
		e->set_sink_port(
			g->get_actor_prop_key(snk, i->first));
	}
	
	return 0;
}

int Parser::read_productions(stringstream & stream, Graph * g) {

	int ret;
	string ratelist, edgename, rate;
	read_str(stream, "{");	
	getline(stream, ratelist, '}');
	
	topology = topology + "production {\n"+ratelist+"\n}\n";

	trim_str(ratelist);

	istringstream ss(ratelist);
	while (getline(ss, rate, ';')) {
		ret = add_production_rate(rate,g);
		if (ret < 0)
			return ret;		
	}
	return 0;
}


int Parser::read_consumptions(stringstream & stream, Graph * g) {

	int ret;
	string ratelist, edgename, rate;
	read_str(stream, "{");	
	getline(stream, ratelist, '}');

	topology = topology + "consumption {\n"+ratelist+"\n}\n";
	
	trim_str(ratelist);

	istringstream ss(ratelist);
	while (getline(ss, rate, ';')) {
		ret = add_consumption_rate(rate,g);
		if (ret < 0)
			return ret;		
	}
	return 0;
}

int Parser::read_tokens(stringstream & stream, Graph * g) {

	int ret;
	string tokenslist, tokens;
	read_str(stream, "{");	
	getline(stream, tokenslist, '}');

	topology = topology + "tokens {\n"+tokenslist+"\n}\n";
	
	trim_str(tokenslist);

	istringstream ss(tokenslist);
	while (getline(ss, tokens, ';')) {
		ret = add_tokens(tokens,g);
		if (ret < 0)
			return ret;		
	}
	return 0;
}

int Parser::read_parameters(stringstream & stream, Graph * g) {

	int ret;
	string params;
	read_str(stream, "{");	
	getline(stream, params, '}');

	parameter = "parameter {"+params+"}";

	trim_str(params);
	
	istringstream ss(params);
	while (getline(ss, params, ';')) {
		ret = add_graph_param(params,g);
		if (ret < 0)
			return ret;		
	}

	return 0;
}

int Parser::read_props(stringstream & stream, Graph * g) {

	int ret;
	string actname, props, prop;
	stream >> actname;
	read_str(stream, "{");	
	getline(stream, props, '}');

	dfactor.insert(std::make_pair(actname, "{"+props+"}"));
	
	trim_str(props);
	
	istringstream ss(props);
	while (getline(ss, prop, ';')) {
		ret = add_actor_prop(actname, prop,g);
		if (ret < 0)
			return ret;		
	}
	return 0;

}

int Parser::read_topology(stringstream & stream, Graph * g) {
        int ret;
	string actorlist, edgelist, actor, edge;
	read_str(stream, "{");
	read_str(stream, "nodes");
	read_str(stream, "=");
	getline(stream, actorlist, ';');

	read_str(stream, "edges");
	read_str(stream, "=");
	getline(stream, edgelist, ';');

	read_str(stream, "}");

	topology = "topology {\n\tnodes = "+actorlist+";\n\tedges = "+edgelist+";\n}\n";

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

int Parser::read_actors(stringstream & stream, Graph * g) {
        int ret;
	string actorlist, actor;
	read_str(stream, "{");
	getline(stream, actorlist, '}');

	trim_str(actorlist);

	istringstream ss(actorlist);
	while (getline(ss, actor, ';')) {
		ret = add_actortype(actor,g);
		if (ret < 0)
			return ret;		
	}
	return 0;
}

int Parser::read_edges(stringstream & stream, Graph * g) {
	int ret;	
	int source, sink;
	string edgelist, edgeline, source_str, sink_str;
	string edge, edge_source, edge_sink;
	read_str(stream, "{");

	getline(stream, edgelist, '}');

	trim_str(edgelist);
	
	istringstream ss(edgelist);

	while (getline(ss, edgeline, ';')) {
		istringstream edl(edgeline);
		getline(edl, edge_source, ',');
		getline(edl, edge_sink, ',');
		getline(edl, source_str, ',');
		getline(edl, sink_str);
		ret = add_edge(edge_source, edge_sink, g);
		if (ret < 0) {
			cout << "error: parsing stream failed.\n";
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
	int ret;
	string edgename="", edgerate="";
	std::istringstream ss(rate);
  	getline(ss, edgename, '=');
	getline(ss, edgerate);
        if (edgename=="" || edgerate=="") {
		cout << "error: edge production rate is not well formatted.\n";
		return -2;
	}
	if (edgerate[0] >= 'a' && edgerate[0] <= 'z') {
		ret = g->set_source_param_rate(edgename, edgerate);	
	} else {
	    try {
		edr = stoi(edgerate);
	    }
	    catch(...) {
		cout << "error: production rate of edge " << edgename << " is not correct.\n";
		return -2;
	    }
	    ret = g->set_source_rate(edgename,edr);
	}
	if (ret == -1)
		cout << "error: edge " << edgename << " is not found.\n";
	return ret;
}

int Parser::add_consumption_rate(const string& rate, Graph * g) {
	int edr=1;
	int ret;
	string edgename="", edgerate="";
	std::istringstream ss(rate);
  	getline(ss, edgename, '=');
	getline(ss, edgerate);
        if (edgename=="" || edgerate=="") {
		cout << "error: edge consumption rate is not well formatted.\n";
		return -2;
	}
	if (edgerate[0] >= 'a' && edgerate[0] <= 'z') {
		ret = g->set_sink_param_rate(edgename, edgerate);	
	} else {
	    try {
		edr = stoi(edgerate);
	    }
	    catch(...) {
		cout << "error: consumption rate of edge " << edgename << " is not correct.\n";
		return -2;
	    }
	    ret = g->set_sink_rate(edgename,edr);
	}
	if (ret == -1)
		cout << "error: edge " << edgename << " is not found.\n";
	return ret;
}

int Parser::add_tokens(const string& edtokens, Graph * g) {
	int tokens=0;
	string edgename="", edgetokens="";
	std::istringstream ss(edtokens);
  	getline(ss, edgename, '=');
	getline(ss, edgetokens);
        if (edgename=="" || edgetokens=="") {
		cout << "error: edge's initial token is not well formatted.\n";
		return -2;
	}
	try {
		tokens = stoi(edgetokens);
	}
	catch(...) {
		cout << "error: intial token of edge " << edgename << " is not correct.\n";
		return -2;
	}
	int ret = g->set_initial_tokens(edgename,tokens);
	if (ret == -1)
		cout << "error: edge " << edgename << " is not found.\n";
	return ret;
}

int Parser::add_graph_param(const string& prop, Graph * g) {
	string key="", val="";
	std::istringstream ss(prop);
  	getline(ss, key, '=');
	getline(ss, val);
        if (key=="" || val=="") {
		cout << "error: parameter is not well formatted.\n";
		return -2;
	}
	int ret = g->add_graph_param(key, val);
	return ret;
}

int Parser::add_actor_prop(const string& actname, const string& prop, Graph * g) {
	string key="", val="";
	std::istringstream ss(prop);
  	getline(ss, key, '=');
	getline(ss, val);
        if (key=="" || val=="") {
		cout << "error: actor property is not well formatted.\n";
		return -2;
	}
	if (key == "host") {
		insert_lookup(val, actname);
	}
	int ret = g->add_actor_prop(actname, key, val);
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
	return ret;
}

Graph * Parser::get_graph() {
	return graph;
}

df::Dataflow * Parser::get_dataflow() {

	df::Dataflow * res;

	std::string srcname, snkname;

	//Create dataflow
	res = new df::Dataflow(graph->get_name());
	
	map<string, string> params = graph->get_graph_params();
	for (auto p : params) {
		res->setProp(p.first, p.second);
	}


	//Create actors
	vector<string> actorlist = graph->get_actors();
	for (auto & acname : actorlist) {
		
		map<string, string> props = graph->get_actor_props(acname);
	        df::Actor * actor; 
		if (props.empty()) {
			cout << acname << " is deployed elsewhere.\n";
			actor = res->createRemoteActor(acname);
		} else {
			string actype = graph->get_actor_type(acname);
			if (actype == "") {
				cout << "error: actor type cannot be unknown.\n";
				cout << "set the property computation of actor " << acname << "\n";
				return nullptr;
			}
			actor = res->createActor(actype, acname);
			for (auto p : props) {
				actor->setProp(p.first, p.second);
			}
		}
	}

	//Create edges
	vector<string> edgelist = graph->get_edges();
	for (auto & edname : edgelist) {
		srcname = graph->get_source_name(edname);
		snkname = graph->get_sink_name(edname); 
		df::Edge * e = res->createEdge(edname, srcname, snkname);
		e->setSourceRate(graph->get_source_rate(edname));
		e->setSinkRate(graph->get_sink_rate(edname));
		e->setSourcePort(graph->get_source_port(edname));
		e->setSinkPort(graph->get_sink_port(edname));
		e->setInitialTokens(graph->get_initial_tokens(edname));
	}	

	return res;
}
