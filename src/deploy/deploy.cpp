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

#include "deploy.h"

Deploy::Deploy(int argc, char * argv[], Parser * p) {

	if (argc != 2) {
		cout << "usage: " << argv[0] << " <file.df>\n";
		exit(0);
	}
	parser = p;		
	parser->load_from_file(argv[1]);
	graph = parser->get_graph();

	sock = new df::ClientSocket("tmf-deploy");
}

int Deploy::run() {

	//Create dataflow
	df::Dataflow dataflow(graph->get_name());
	
	map<string, string> params = graph->get_graph_params();
	for (auto p : params) {
		dataflow.setProp(p.first, p.second);
	}

	//Create actors
	vector<string> actorlist = graph->get_actors();
	for (auto & acname : actorlist) {
		
		map<string, string> props = graph->get_actor_props(acname);
		string actype = graph->get_actor_type(acname);
		if (actype == "") {
			cout << "error: actor type cannot be unknown.\n";
			cout << "set the property computation of actor " << acname << "\n";
			return -1;
		}
		df::Actor * actor = dataflow.createActor(actype, acname);
		for (auto p : props) {
			actor->setProp(p.first, p.second);
		}
	}

	//Initialize discovery
	dataflow.runDiscovery();

	std::string msg;
	auto ips = parser->get_ips();

	for (auto&& ip : ips) {
		sock->connect(ip, SERVER_PORT);
		//msg = parser->df_all();
		msg = parser->df_byip(ip);	
		sock->send(msg.c_str(), msg.size());
		sock->close();
	}

	dataflow.waitDiscovery();
	
	return 0;
}
