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

#include "server.h"

Server::Server(Parser * p) {
	sock = new df::ServerSocket("tmf-server");	
	parser = p;
}

int Server::init() {

	int bufsize = 100000;
	char buf[bufsize];

	sock->listen(7000);
	std::cout << "waiting tmf-deploy...\n";
	sock->accept();
	sock->recv(buf, bufsize);
	std::cout << buf << std::endl;
	parser->load_from_string(buf);
	graph = parser->get_graph();

	sock->clnclose();
	sock->srvclose();

	return 0;
}

int Server::run() {
	
	map<string, df::Actor *> actormap;

	//Create dataflow
	df::Dataflow dataflow(graph->get_name());
	
	map<string, string> params = graph->get_graph_params();
	for (auto p : params) {
		dataflow.setProp(p.first, p.second);
	}

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
		actormap[acname] = actor;
	}

	//Initialize dataflow
	dataflow.init();

	//Connect actors
	vector<string> edgelist = graph->get_edges();
	for (auto & ed : edgelist) {
		df::Actor * src = actormap[graph->get_source_name(ed)];
		df::Actor * snk = actormap[graph->get_sink_name(ed)];
		dataflow.connectActors(src, snk, ed, graph->get_source_rate(ed), graph->get_sink_rate(ed));
	}

	//Run dataflow
  	dataflow.run();

	//Destructor of dataflow is called.

	return 0;
}
