/*
 *   RDF
 *   Copyright (C) INRIA - Orange Labs
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

#include "rule.h"

Rule::Rule(string name) {
	this->name = name;
	l = new Graph();
	r = new Graph();
	res = new Graph();
	connectivity = true;
	consistency = true;
	liveness = true;
	matching = true;
	disappearing_actor = true;
}

Graph * Rule::left() {
	return l;
}

Graph * Rule::right() {
	return r;
}

Graph * Rule::result() {
	return res;
}

string Rule::get_name() {
	return name;
}

int Rule::process(Graph * graph) {
	g = graph;
	disappearing_actor_check();
	matching_check();
	connectivity_check();
	consistency_check();
	liveness_check();
	apply();
	return 0;
}

string Rule::benchmark() {
	string bench="";
	double conc, cons, live, rconc, rcons, rlive;
	time_t t_start = clock();
	connectivity_check();
	conc = (double)(clock() - t_start)/CLOCKS_PER_SEC;
	t_start = clock();
	consistency_check();
	cons = (double)(clock() - t_start)/CLOCKS_PER_SEC;
	t_start = clock();
	liveness_check();
	live = (double)(clock() - t_start)/CLOCKS_PER_SEC;
	apply();
	t_start = clock();
	result_connectivity();
	rconc = (double)(clock() - t_start)/CLOCKS_PER_SEC;
	t_start = clock();
	result_consistency();
	rcons = (double)(clock() - t_start)/CLOCKS_PER_SEC;
	t_start = clock();
	result_liveness();
	rlive = (double)(clock() - t_start)/CLOCKS_PER_SEC;
	bench = name + ", " + to_string(conc) + ", " + to_string(rconc) + ", " +
		to_string(cons) + ", " + to_string(rcons) +  ", " +
		to_string(live) + ", " + to_string(rlive);

	return bench;
}

int Rule::verify() {
	if (!matching) {
		cout << "Matching is not found.\n";
		return -1;
	}
	if (!disappearing_actor) {
		cout << "A disappearing actor has implicite edges.\n";
		return -1;
	}
	if (connectivity && consistency && liveness)
		cout << "The rule is valid.\n";
	else {
		cout << "The rule is not valid because of ";
		if (!connectivity)
			cout << "connectivity, ";
		if (!consistency)
			cout << "consistency, ";
		if (!liveness)
			cout << "liveness, ";
		cout << "\n";
		return -1;
	}
	return 0;
}

int Rule::apply() {
	int ret;
	res->set_name(g->get_name()+"."+name);
	
	if (matching && disappearing_actor && connectivity && consistency && liveness) {
		
		auto gactors = g->get_actors();
		//Add common actors and appearing actors to the result.
		for (auto c : gactors) {
			if (find(disapp_actors.begin(), disapp_actors.end(), c) == disapp_actors.end()) {
				auto type = g->get_actor_type(c);
				res->add_actor(c, type);
			}
		}
		for (auto a : app_actors) {
			auto type = r->get_actor_type(a);
			res->add_actor(a,type);
		}
		
		//Remove left edges.
		auto edges = g->get_edges();
		auto leftedges = l->get_edges();
		for (auto edge : edges) {
			if (find(leftedges.begin(), leftedges.end(), edge) == leftedges.end()) {
				auto src = g->get_source_name(edge);
				auto snk = g->get_sink_name(edge);
				auto src_rate = g->get_source_rate(edge);
				auto snk_rate = g->get_sink_rate(edge);
				ret = res->add_edge(src, snk);
				res->set_source_rate(edge, src_rate);
				res->set_sink_rate(edge, snk_rate);
			}
		}
		
		//Add right edges.
		auto rightedges = r->get_edges();
		vector<string> resedges;
		for (auto edge : rightedges) {
			auto src = r->get_source_name(edge);
			auto snk = r->get_sink_name(edge);
			auto src_rate = r->get_source_rate(edge);
			auto snk_rate = r->get_sink_rate(edge);
			resedges = res->get_edges();
			if (find(resedges.begin(), resedges.end(), edge) == resedges.end())	
				res->add_edge(src, snk);
			res->set_source_rate(edge, src_rate);
			res->set_sink_rate(edge, snk_rate);
		}
		
		return 0;
	}
	
	return -1;
}

void Rule::extract_common_actors() {
	vector<string> lactors = l->get_actors();
	vector<string> ractors = r->get_actors();

	for (auto ract : ractors) {
		if (find(lactors.begin(), lactors.end(), ract) != lactors.end())
			common_actors.push_back(ract);
		else
			app_actors.push_back(ract);
	}

	for (auto lact : lactors) {
		if (find(ractors.begin(), ractors.end(), lact) == ractors.end())
			disapp_actors.push_back(lact);
	}
}

bool Rule::disappearing_actor_check() {
	extract_common_actors();
	auto edges = g->get_edges();
	auto leftedges = l->get_edges();
	for (auto ed : edges) {
		auto source_actor = g->get_source_name(ed);
		auto sink_actor = g->get_sink_name(ed);
		if (
			(find(disapp_actors.begin(), disapp_actors.end(), source_actor) 
			!= disapp_actors.end()) ||
		    	(find(disapp_actors.begin(), disapp_actors.end(), sink_actor) 
			!= disapp_actors.end())  
		   ) {
			if (!match(l, ed)) {
				disappearing_actor = false;
				return disappearing_actor;
			}	
		}
	}
	disappearing_actor = true;
	return disappearing_actor;
}

bool Rule::match(Graph * g, string edge) {
	auto gedges = g->get_edges();
	if (find(gedges.begin(), gedges.end(), edge) == gedges.end()) {
		return false;
	}
	if ((g->get_source_rate(edge) != l->get_source_rate(edge)) ||
	     (g->get_sink_rate(edge) != l->get_sink_rate(edge)) ||
	     (g->get_source_type(edge) != l->get_source_type(edge)) || 
	     (g->get_sink_type(edge) != l->get_sink_type(edge)) ) {
		return false;
	}
	return true;
}

bool Rule::matching_check() {
	auto ledges = l->get_edges();
	for (auto le : ledges) {
		if (!match(g, le)) {
			matching = false;
			return matching;
		}
	}
	matching = true;
	return matching;
}

bool Rule::connectivity_check() {
	if (!r->connected()) {
		connectivity = false;
		return connectivity;
	}
	connectivity = true;
	return connectivity;
}

bool Rule::result_connectivity() {
	if (res->empty() || !res->connected())
		return false;
	return true;
}
	
bool Rule::consistency_check() {
	int ret;
	//Set firings belonging to the RDF data graph.
	for(auto ac : g->get_actors()) {
		//l->set_firing(ac, g->get_firing(ac));
		r->set_firing(ac, g->get_firing(ac));
	}
	//ret = l->solve();
	//if (ret < 0) {
	//	matching = false;
	//	consistency = false;
	//}
	ret = r->solve();
	if (ret < 0) {
		consistency = false;
		return consistency;
	}
	consistency = true;
	return consistency;
}

bool Rule::result_consistency() {
	int ret;
	if (res->empty())
		return false;
	ret = res->resolve();
	if (ret < 0)
		return false;
	return true;
}

bool Rule::liveness_check() {

	vector<vector<string>> lorder = l->order();
	vector<vector<string>> rorder = r->order();
	vector<string> lactors = l->get_actors();
	vector<string> ractors = r->get_actors();

	string order;
	vector<string> lordercommon;
	vector<string> rordercommon;

	//Keep only common actors.
	for (auto lo : lorder) {
		order = "";
		for (auto e : lo) {
			if (find(ractors.begin(), ractors.end(), e) != ractors.end()) {
				order = order + e;
			}
		}
		lordercommon.push_back(order);
	}
	for (auto ro : rorder) {
		order = "";
		for (auto e : ro) {
			if (find(lactors.begin(), lactors.end(), e) != lactors.end()) {
				order = order + e;
			}
		}
		rordercommon.push_back(order);
	}
			
	//Check that for each order in lordercommon, there exists an rordercommon.
	for (auto loc : lordercommon) {
		if (find(rordercommon.begin(), rordercommon.end(), loc) == rordercommon.end()) {
			liveness = false;
			return liveness;
		}
	}
	liveness = true;
	return liveness;
}

bool Rule::result_liveness() {
	if (res->empty())
		return false;
	auto ret = res->order();
	if (ret.empty())
		return false;
	return true;
}
