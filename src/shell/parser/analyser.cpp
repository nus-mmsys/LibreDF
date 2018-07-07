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

#include "analyser.h"

int Analyser::gcd(int a, int b) {
	int r;
	while (b != 0) {
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}

vector<Actor *> Analyser::findsources() {
	vector<Actor *> sourcelist;
	for (auto it = graph->actor_begin(); it != graph->actor_end(); it++) {
		if (it->second->get_iedges().size() == 0) {
			sourcelist.push_back(it->second);
		}
	}
	if (sourcelist.size() == 0) {
		sourcelist.push_back(graph->actor_begin()->second);
	}
	return sourcelist;
}

void Analyser::dfs_visited_actors(Actor * curr, int & visited) {

	visited++;
	Actor * adj;

	for (auto e : curr->get_oedges()) {
		if (!e->get_visited()) {
			adj = e->get_sink_actor();
			e->set_visited(true);
			dfs_visited_actors(adj, visited);
		}
	}

	for (auto e : curr->get_iedges()) {
		if (!e->get_visited()) {
			adj = e->get_source_actor();
			e->set_visited(true);
			dfs_visited_actors(adj, visited);
		}
	}
	return;
}

int Analyser::dfs(Actor * curr, int num, int den) {

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

	for (auto e : curr->get_oedges()) {
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

	for (auto e : curr->get_iedges()) {
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

vector<vector<Actor *>> Analyser::combinations(vector<Actor *> heads) {

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

vector<vector<Actor *>> Analyser::findorders() {

	vector<vector<Actor *>> orderlist;
	vector<Actor *> heads;

	if (graph->actor_size() == 0)
		return orderlist;

	for (auto it = graph->actor_begin(); it != graph->actor_end(); it++)
		it->second->set_visited(false);

	heads = findsources();

	orderlist = findorders_from(heads);
	return orderlist;
}

vector<vector<Actor *>> Analyser::findorders_from(vector<Actor *> heads) {

	vector<Actor *> order;
	vector<vector<Actor *>> combs;
	vector<vector<Actor *>> rests;
	vector<Actor *> restheads;
	vector<vector<Actor *>> orderlist;

	if (heads.size() == 0)
		return orderlist;

	for (auto h: heads) {
		vector<Edge *> edges = h->get_oedges();
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
		vector<Edge *> ed = (*i)->get_iedges();
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

vector<string> Analyser::sas() {
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

int Analyser::resolve() {
	
	for (auto it = graph->actor_begin(); it != graph->actor_end(); it++)
 		it->second->set_firing(0);
	return solve();
}

int Analyser::solve() {
	
	int lcm;
	int ret;
	if (graph->actor_size() == 0)
		return 0;

	for (auto it = graph->edge_begin(); it != graph->edge_end(); it++)
		it->second->set_visited(false);

	Actor * curr = graph->actor_begin()->second;
	if (curr->get_firing() == 0) {
		curr->set_firing(1);
		ret = dfs(curr, 1, 1);
	} else {
		ret = dfs(curr, curr->get_num(), curr->get_den());
	}
	if (ret >= 0) {
		lcm = 0;
		
		for (auto it = graph->actor_begin(); it != graph->actor_end(); it++) {
			if (lcm == 0)
				lcm = it->second->get_den();
			else 
				lcm = lcm / gcd(lcm, it->second->get_den()) * it->second->get_den();
		}
		for (auto it = graph->actor_begin(); it != graph->actor_end(); it++) {
			it->second->set_firing(lcm * it->second->get_num() / it->second->get_den());
		}
	}
	return ret;
}

bool Analyser::connected() {
	int visited=0;
	
	for (auto it = graph->edge_begin(); it != graph->edge_end(); it++)
		it->second->set_visited(false);
	dfs_visited_actors(graph->actor_begin()->second, visited);
	if (visited == graph->actor_size())
		return true;
	return false;
}

vector<vector<string>> Analyser::order() {
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
