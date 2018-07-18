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

#include "graph.h"

void Graph::set_name(string name) {
	this->name = name;
}

string Graph::get_name() {
	return this->name;
}

Actor * Graph::create_actor(string name, string type) {
	if (actors.find(name) == actors.end()) {
		Actor * ac = new Actor(name, type);
		actors[name] = ac;
	}
	return actors[name];
}

Actor * Graph::get_actor(string actor) {
	if (actors.find(actor) == actors.end()) {
		return nullptr;
	}
	return actors[actor];
}

Edge * Graph::create_edge(string name) {
	if (edges.find(name) == edges.end()) {
		Edge * ed = new Edge(name);
		edges[name] = ed;
	}
	return edges[name];
}

Edge * Graph::get_edge(Actor * source, Actor * sink) {
	string edgename = source->get_name() + sink->get_name();
	if (edges.find(edgename) == edges.end()) {
		return nullptr;
	}
	return edges[edgename];
}

int Graph::gcd(int a, int b) {
	int r;
	while (b != 0) {
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}

int Graph::add_actor(string name, string type) {
	Actor * ac = create_actor(name, type);
	if (ac == nullptr)
		return -1;
	return 0;
}

int Graph::add_graph_param(string key, string val) {
	if (params.find(key) != params.end()) {
		return -1;
	}
	params[key] = val;
	return 0;
}

int Graph::add_actor_prop(string actname, string key, string val) {
	Actor * ac;
	if (actors.find(actname) == actors.end()) {
		return -1;
	}
	ac = actors[actname];
	ac->set_prop(key, val);
	return 0;
}

int Graph::set_actor_type(string name, string type) {
	Actor * ac = get_actor(name);
	if (ac != nullptr) {
		ac->set_type(type);
		return 0;
	}
	return -1;
}

string Graph::get_actor_type(string name) {
	Actor * ac = get_actor(name);
	if (ac != nullptr)
		return ac->get_type();
	return "";
}

int Graph::add_edge(string edgename, string edge_source, string edge_sink) {

	if (edges.find(edgename) != edges.end())
		return -2;	

	Edge * edge = create_edge(edgename);
			
	Actor * sourceactor = get_actor(edge_source);
	if (sourceactor == nullptr)
		return -1;
	Actor * sinkactor = get_actor(edge_sink);
	if (sinkactor == nullptr)
		return -1;
	Port * op = sourceactor->create_oport();
	Port * ip = sinkactor->create_iport();

	edge->connect_source(op);
	edge->connect_sink(ip);

	edge->set_source_actor(sourceactor);
	edge->set_sink_actor(sinkactor);

	return 0;
}


int Graph::add_edge(string edge_source, string edge_sink) {

	string edgename = edge_source + edge_sink;
        return add_edge(edgename, edge_source, edge_sink);
}

vector<Edge *> Graph::get_iedges(Actor * ac) {
	vector <Edge *> res;
	for (auto & e : edges) {
		if (e.second->get_sink_actor()->get_name() == ac->get_name())
			res.push_back(e.second);
	}
	return res;
}

vector<Edge *> Graph::get_oedges(Actor * ac) {
	vector <Edge *> res;
	for (auto & e : edges) {
		if (e.second->get_source_actor()->get_name() == ac->get_name())
			res.push_back(e.second);
	}
	return res;
}

vector<Actor *> Graph::findsources() {
	vector<Actor *> sourcelist;
	for (auto n : actors) {
		if (n.second->iport_size() == 0) {
			sourcelist.push_back(n.second);
		}
	}
	if (sourcelist.size() == 0) {
		sourcelist.push_back(actors.begin()->second);
	}
	return sourcelist;
}

vector<vector<string>> Graph::order() {
	vector<vector<string>> orderliststr;
	vector<string> orderstr;
	vector<vector<Actor *>> orderlist = findorders();
	for (auto order : orderlist) {
		orderstr.clear();
		for (auto o : order)
			orderstr.push_back(o->get_name());
		orderliststr.push_back(orderstr);
	}
	return orderliststr;
}

void Graph::dfs_visited_actors(Actor * curr, int & visited) {

	visited++;
	Actor * adj;

	for (auto e : get_oedges(curr)) {
		if (!e->get_visited()) {
			adj = e->get_sink_actor();
			e->set_visited(true);
			dfs_visited_actors(adj, visited);
		}
	}

	for (auto e : get_iedges(curr)) {
		if (!e->get_visited()) {
			adj = e->get_source_actor();
			e->set_visited(true);
			dfs_visited_actors(adj, visited);
		}
	}
	return;
}

int Graph::dfs(Actor * curr, int num, int den) {

	Actor * adj;
	if (num % den == 0) {
		num = num / den;
		den = 1;
	}
	if (den % num == 0) {
		den = den / num;
		num = 1;
	}

	if ((curr->get_num() == 0) || (float(num) / curr->get_num() == float(den) / curr->get_den())) {
		curr->set_numden(num, den);
	} else {
		return -1;
	}

	for (auto e : get_oedges(curr)) {
		if (!e->get_visited()) {
			adj = e->get_sink_actor();
			e->set_visited(true);
			num = e->get_source_rate() *
				e->get_source_actor()->get_num(); 
			den = e->get_sink_rate() * e->get_source_actor()->get_den(); 
			int ret = dfs(adj, num, den);
			if (ret == -1)
				return -1;
		}
	}

	for (auto e : get_iedges(curr)) {
		if (!e->get_visited()) {
			adj = e->get_source_actor();
			e->set_visited(true);
			num = e->get_sink_rate() * 
				e->get_sink_actor()->get_num(); 
			den = e->get_source_rate() * e->get_sink_actor()->get_den();
			int ret = dfs(adj, num, den);
			if (ret == -1)
				return -1;
		}
	}
	return 0;
}

int Graph::resolve() {
	for (auto ac : actors)
 		ac.second->set_firing(0);
	return solve();
}

int Graph::solve() {
	
	int lcm;
	int ret;
	if (actors.size() == 0)
		return 0;

	for (auto  e : edges)
		e.second->set_visited(false);

	Actor * curr = actors.begin()->second;
	if (curr->get_firing() == 0) {
		curr->set_firing(1);
		ret = dfs(curr, 1, 1);
	} else {
		ret = dfs(curr, curr->get_num(), curr->get_den());
	}
	if (ret >= 0) {
		lcm = 0;
		for (auto ac : actors) {
			if (lcm == 0)
				lcm = ac.second->get_den();
			else 
				lcm = lcm / gcd(lcm, ac.second->get_den()) * ac.second->get_den();
		}
		for (auto ac : actors) {
			ac.second->set_firing(lcm * ac.second->get_num() / ac.second->get_den());
		}
	}
	return ret;
}

bool Graph::connected() {
	int visited=0;
	for (auto  e : edges)
		e.second->set_visited(false);
	dfs_visited_actors(actors.begin()->second, visited);
	if (visited == actors.size())
		return true;
	return false;
}
vector<vector<Actor *>> Graph::combinations(vector<Actor *> heads) {

	vector<vector<Actor *>> combs;
	vector<vector<Actor *>> res;
	vector<Actor *> order1;
	vector<Actor *> order2;

	if (heads.size() == 1) {
		combs.push_back(heads);
		return combs;
	}
	Actor * b = heads.back();
	heads.pop_back();
	combs = combinations(heads);
	for (auto comb : combs) {
		order1.clear();
		order2.clear();
		order1.push_back(b);
		for (auto c : comb) {
			order1.push_back(c);
			order2.push_back(c);
		}	
		order2.push_back(b);
		res.push_back(order1);
		res.push_back(order2);
	}
	return res;
}

vector<vector<Actor *>> Graph::findorders() {

	vector<vector<Actor *>> orderlist;
	vector<Actor *> heads;

	if (actors.size() == 0)
		return orderlist;

	for (auto ac : actors)
		ac.second->set_visited(false);

	heads = findsources();

	orderlist = findorders_from(heads);
	return orderlist;
}

vector<vector<Actor *>> Graph::findorders_from(vector<Actor *> heads) {

	vector<Actor *> order;
	vector<vector<Actor *>> combs;
	vector<vector<Actor *>> rests;
	vector<Actor *> restheads;
	vector<vector<Actor *>> orderlist;

	if (heads.size() == 0)
		return orderlist;

	for (auto h: heads) {
		vector<Edge *> edges = get_oedges(h);
		h->set_visited(true);		
		for (auto e : edges)
			if (find(restheads.begin(), restheads.end(), e->get_sink_actor()) == restheads.end() 
					&& e->get_sink_actor()->get_visited() == false) {
				restheads.push_back(e->get_sink_actor());
			}
	}

	auto i = restheads.begin();
	bool found = false;
	while (i != restheads.end()) {
		found = false;
		vector<Edge *> ed = get_iedges(*i);
		for (auto e : ed) {
			if (find(restheads.begin(), restheads.end(), e->get_source_actor()) != restheads.end()) {
				i = restheads.erase(i);
				found = true;
				break;
			}
		}
		if (!found)
			i++;
	}

	// Find the possible combinations of heads.
	combs = combinations(heads);
	rests = findorders_from(restheads);

	if (rests.size() == 0) {
		return combs;
	}

	for (auto comb : combs) {
		for (auto rest : rests) {
			order.clear();
			order.insert(order.begin(), comb.begin(), comb.end());
			order.insert(order.end(), rest.begin(), rest.end());
	 		orderlist.push_back(order);
		}
	}

	return orderlist; 	
}

vector<string> Graph::sas() {
	vector<string> sasliststr;
	vector<Schedule *> schedules;
	vector<vector<Actor *>> orders = findorders();

	solve();	
	for (auto order : orders) {
		Schedule * s = new Schedule();
		s->insert_order(order);
		schedules.push_back(s);
	}
	for (auto s : schedules) {
		string sasstr = s->to_string();
		sasliststr.push_back(sasstr);
	}
	
	return sasliststr;
}

int Graph::set_firing(string name, int firing) { 
	if (actors.find(name) == actors.end())
		return -1;
	actors[name]->set_firing(firing);
	return 0;
}

int Graph::get_firing(string name) {
	if (actors.find(name) == actors.end())
		return -1;
	return actors[name]->get_firing();
}

int Graph::set_source_rate(string edgename, int rate) { 
	if (edges.find(edgename) == edges.end())
		return -1;
	edges[edgename]->set_source_rate(rate);
	return 0;
}

int Graph::get_source_rate(string edgename) { 
	if (edges.find(edgename) == edges.end())
		return -1;
	return edges[edgename]->get_source_rate();
}

string Graph::get_source_name(string edgename) { 
	if (edges.find(edgename) == edges.end())
		return "";
	return edges[edgename]->get_source_actor()->get_name();
}

string Graph::get_source_type(string edgename) { 
	if (edges.find(edgename) == edges.end())
		return "";
	return edges[edgename]->get_source_actor()->get_type();
}

int Graph::set_sink_rate(string edgename, int rate) { 
	if (edges.find(edgename) == edges.end())
		return -1;
	edges[edgename]->set_sink_rate(rate);
	return 0;
}

int Graph::get_sink_rate(string edgename) { 
	if (edges.find(edgename) == edges.end())
		return -1;
	return edges[edgename]->get_sink_rate();
}

string Graph::get_sink_name(string edgename) { 
	if (edges.find(edgename) == edges.end())
		return "";
	return edges[edgename]->get_sink_actor()->get_name();
}

string Graph::get_sink_type(string edgename) { 
	if (edges.find(edgename) == edges.end())
		return "";
	return edges[edgename]->get_sink_actor()->get_type();
}

vector<string> Graph::get_actors() {
       	vector<string> actorlist;
	for (auto ac : actors)
		actorlist.push_back(ac.second->get_name());
	return actorlist;
}

vector<string> Graph::get_edges() {
       	vector<string> edgelist;
	for (auto ed : edges)
		edgelist.push_back(ed.second->get_name());
	return edgelist;
}

string Graph::get_actor_prop(string actname, string prop) {
	string res = "";
	if (actors.find(actname) == actors.end())
		return res;
	return actors[actname]->get_prop(prop);
}

map<string, string> Graph::get_graph_params() {
	return params;
}

map<string, string> Graph::get_actor_props(string actname) {
	map<string, string> res;
	if (actors.find(actname) == actors.end())
		return res;
	return actors[actname]->get_props();
}

bool Graph::empty() {
	if (actors.size() == 0)
		return true;
	return false;
}

/*
vector<vector<string>> Graph::path() {
	vector<vector<string>> pathliststr;
	vector<string> pathstr;
	findpaths();
	for (auto path : pathlist) {
		pathstr.clear();
		for (auto p : path)
			pathstr.push_back(p->get_name());
		pathliststr.push_back(pathstr);
	}
	return pathliststr;
}
*/

/*
void Graph::findpaths() {

	vector<Actor *> heads;
	vector<Actor *> path;
	pathlist.clear();

	if (actors.size() == 0)
		return;

	for (auto e : edges)
		e.second->set_visited(false);

	heads = findsources();
	
	if (heads.size() == 0)
		heads.push_back(actors.begin()->second);

	for (auto h : heads)
		findpaths_from(h, path);
}
*/

/*
void Graph::findpaths_from(Actor * head, vector<Actor *> path) {

	vector<vector<Actor *>> pathlisttmp;
	vector<Edge *> edges = head->get_oedges();
	
	
	if (edges.size() == 0 || find(path.begin(), path.end(), head) != path.end()) {
		path.push_back(head);
		pathlist.push_back(path);
		return;
	}

	bool found = false;
	path.push_back(head);
	for (auto e : edges) {
		if (!e->get_visited()) {
			e->set_visited(true);
			found = true;
			findpaths_from(e->get_sink_actor(), path);
		}
	}

	if (!found)
		pathlist.push_back(path);	
}
*/

/*
void Graph::findpaths2() {
	
	pathlist.clear();
	if (actors.size() == 0)
		return;

	vector<Actor *> heads = findsources();;

	if (heads.size() == 0)
		heads.push_back(actors.begin()->second);

	vector<Actor *> path;
	for (int i=0; i<heads.size(); i++) {
		path.clear();

		for (auto  e : edges) {
			e.second->set_visited(false);
		}
				
		Actor * curr = heads[i];
		path.push_back(curr);
		bool forward = false;
		bool loopdetect = false;
		while (true) {
			bool found = false;
			for (auto e : curr->get_oedges()) {
				if (!e->get_visited()) {
					curr = e->get_sink_actor();
					e->set_visited(true);
					path.push_back(curr);
					found = true;
					forward = true;
					break;
				}
			}
			for (int p=0; p<path.size()-1; p++) {
				if (path[p]->get_name() == path[path.size()-1]->get_name()) {
					loopdetect = true;
					break;
				}
			}

			if (curr->get_oedges().size() == 0 || (!found && forward) || loopdetect) {
				pathlist.push_back(path);
			}
			
			if (!found || curr->get_oedges().size() == 0 || loopdetect) {
				path.pop_back();
				if (path.empty()) {
					break;
				}
				curr = path[path.size()-1];
				forward = false;
				loopdetect = false;
			}
		}
	}

}
*/

/*
int Graph::solve2() {
	int num, den, g;
	findpaths();
	for (auto ac : actors)
 		ac.second->set_firing(0);
	for (auto path : pathlist) {
		if (path[0]->get_firing() == 0)
			path[0]->set_firing(1);
		for (int i=1; i<path.size(); i++) {
			Edge * edge = get_edge(path[i-1], path[i]);
			num = path[i-1]->get_firing() * edge->get_source_rate();
			den = edge->get_sink_rate();
			g = gcd(num,den);
			num = num/g;
			den = den/g;
			
			if ((path[i]->get_name() == path[0]->get_name()) 
					&& (num != path[0]->get_firing()))
				return -1;
	
			if (path[i]->get_firing() == 0)
				path[i]->set_firing(num);
			else if (num * den != path[i]->get_firing())
				return -1;
			for (int j=0; j<i; j++)
				path[j]->set_firing(den * path[j]->get_firing());
			
		}
	}
	return 0;
}
*/
