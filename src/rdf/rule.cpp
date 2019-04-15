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

bool Rule::is_name(const std::string& s)
{
	return (s[0] >= 'A' && s[0] <= 'Z');
}

bool Rule::is_variable(const std::string& s)
{
	return (s[0] >= 'a' && s[0] <= 'z');
}

void Rule::preprocess() {
	vector<string> actors = l->get_actors();
	string type;
	for (auto ac : actors) {
		type = l->get_actor_type(ac);

		if (is_name(ac)) {
			nameconst.push_back(ac);
		} else if (is_variable(ac)) {
			namevar.insert(make_pair(ac,""));
		} else {
			cout << "Invalid actor name " << ac << endl;
		}	
	
		if (is_variable(type)) {
			typevar.insert(make_pair(ac,""));
		} 
	}
}

Graph * Rule::apply(Graph * graph) {
	g = graph;
	res->clear();	
	if (matching_check()) {
		disappearing_actor_check();
		apply();
		return res;
	} else
		cout << "Cannot find a matching.\n";
	return nullptr;
}

int Rule::process(Graph * graph) {
	connectivity_check();
	consistency_check();
	liveness_check();
	g = graph;
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
	apply(g);
	if (!matching) {
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
		vector<string> disact;
		for (auto c : disapp_actors) {
			auto gc = c;
			if (is_variable(c))
				gc = namevar[c];
			disact.push_back(gc);
		}
		for (auto c : gactors) {
			if (find(disact.begin(), disact.end(), c) == disact.end()) {
				auto type = g->get_actor_type(c);
				res->add_actor(c, type);
				res->set_actor_props(c, g->get_actor_props(c));
			}
		}
		for (auto c : common_actors) {
			auto gc = c;
			if (is_variable(c))
				gc = namevar[c];
			res->replace_actor_props(gc, r->get_actor_props(c));
		}
		int idx=1;
		for (auto a : app_actors) {
			if (is_name(a))
				cout << "Appearing actor cannot be named.\n";
			auto type = r->get_actor_type(a);
			auto gtype = type;
			if (is_variable(type))
				gtype = typevar[type];
			string newname;
			do {
				srand((unsigned) time(0)+idx);
				idx++;
				newname = toupper(a[0]); 
				newname += to_string(rand()%32);
			} while (g->contains_actor(newname));
			namevar[a] = newname;
			res->add_actor(newname,gtype);
			res->set_actor_props(newname, r->get_actor_props(a));
		}
		
		//Add original edges.
		auto edges = g->get_edges();
		auto leftedges = l->get_edges();
		bool found = false;
		for (auto edge : edges) {
			auto src = g->get_source_name(edge);
			auto snk = g->get_sink_name(edge);
			found = false;
			for (auto ledge : leftedges) {
				auto lsrc = l->get_source_name(ledge);
				auto lsnk = l->get_sink_name(ledge);
				if (is_variable(lsrc))
					lsrc = namevar[lsrc];
				if (is_variable(lsnk))
					lsnk = namevar[lsnk];
				if (src==lsrc && snk==lsnk) {
					found = true;
					break;
				}

			}
			if (found==false) {
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
		int src_rate, snk_rate;
		for (auto edge : rightedges) {
			auto src = r->get_source_name(edge);
			auto snk = r->get_sink_name(edge);
			if (is_variable(src))
				src = namevar[src];
			if (is_variable(snk))
				snk = namevar[snk];

			auto src_rate_p = r->get_source_rate_p(edge);
			auto snk_rate_p = r->get_sink_rate_p(edge);
			
			if (is_variable(src_rate_p)) {
				src_rate = ratevar[src_rate_p];
			} else
				src_rate = r->get_source_rate(edge);

			if (is_variable(snk_rate_p)) {
				snk_rate = ratevar[snk_rate_p];
			} else
				snk_rate = r->get_sink_rate(edge);

			resedges = res->get_edges();
			if (find(resedges.begin(), resedges.end(), edge) == resedges.end())	
			{
				res->add_edge(src, snk, src_rate, snk_rate);
			}
		}
		res->solve();	
		return 0;
	}
	
	return -1;
}

void Rule::extract_common_actors() {
	vector<string> lactors = l->get_actors();
	vector<string> ractors = r->get_actors();
	common_actors.clear();
	app_actors.clear();
	disapp_actors.clear();
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
	for (auto d : disapp_actors) {
		auto dg = d;
		if (is_variable(d)) {
			dg = namevar[d];
		}

		auto lpred = l->get_pred(d);
		auto lsucc = l->get_succ(d);
	
		auto gpred = g->get_pred(d);
		auto gsucc = g->get_succ(d);

		if (lpred.size() != gpred.size()
			&& lsucc.size() != lsucc.size()) {
			disappearing_actor = false;
			return disappearing_actor;
		}
	}
	disappearing_actor = true;
	return disappearing_actor;
}

bool Rule::node_match(string lnode, string gnode) {
	string ltype = l->get_actor_type(lnode);
	string gtype = g->get_actor_type(gnode);
	return ( (is_name(lnode) && lnode == gnode)
	       ||(is_variable(lnode) && is_variable(ltype))
	       ||(is_variable(lnode) && is_name(ltype) && ltype==gtype));
}

bool Rule::edge_match(string lc, string lk, string gc, string gk) {
	string led = l->get_edge_name(lc, lk);
	string ged = g->get_edge_name(gc, gk);
	string lcr = l->get_source_rate_p(led);
	string lkr = l->get_sink_rate_p(led);
	string gcr = g->get_source_rate_p(ged);
	string gkr = g->get_sink_rate_p(ged);
	return ( (is_variable(lcr) && is_variable(lkr))
	       ||(is_variable(lcr) && lkr == gkr)
	       ||(is_variable(lkr) && lcr == gcr)
	       ||(lkr == gkr && lcr == gcr));
}

map<string, string> Rule::matching_from(string lnode, string gnode, map<string, string> matchmap) {
	map<string,string> res;
	if (node_match(lnode, gnode)) {
		matchmap.insert(make_pair(lnode, gnode));
		if (matchmap.size()==l->actor_size())
			return matchmap;
		vector<string> lpred = l->get_pred(lnode);
		vector<string> gpred = g->get_pred(gnode);
		for (auto lp : lpred) {
			if (matchmap.find(lp)==matchmap.end()) {
				for (auto gp : gpred) {
					if (edge_match(lp, lnode, gp, gnode)) {
					    res = matching_from(lp,gp, matchmap);
					    if (res.size()!=0)
						return res;
					}
				}
			}
		}
		vector<string> lsucc = l->get_succ(lnode);
		vector<string> gsucc = g->get_succ(gnode);
		for (auto ls : lsucc) {
			if (matchmap.find(ls)==matchmap.end()) {
				for (auto gs : gsucc) {
					if (edge_match(lnode, ls, gnode, gs)) {
					    res = matching_from(ls,gs, matchmap);
					    if (res.size()!=0)
						return res;
					}
				}
			}
		}
	}
	return res;
}

bool Rule::matching_check() {
	if (nameconst.size()==0) {
		matching = false;
		return matching;
	}
	map<string, string> matchmap = 
		matching_from(nameconst[0], nameconst[0], map<string,string>());
	
	if (matchmap.size()==0) {
		matching = false;
		return matching;
	}

	for (auto m : matchmap) {
		if (is_variable(m.first)) {
			namevar[m.first] = m.second;
			if (is_variable(l->get_actor_type(m.first))) {
				typevar.insert(make_pair(
					l->get_actor_type(m.first),
					g->get_actor_type(m.second)));
			}

		}
	}

	string lcr, lkr, srconame, snkoname, srcname, snkname, ged;
	int gcr, gkr;
	auto ledges = l->get_edges();
	for (auto led : ledges) {
		lcr = l->get_source_rate_p(led);
		lkr = l->get_sink_rate_p(led);
		srconame = l->get_source_name(led);
		snkoname = l->get_sink_name(led);
		if (is_variable(srconame))
			srcname = namevar[srconame];
		else if (is_name(srconame))
			srcname = srconame;
		if (is_variable(snkoname))
			snkname = namevar[snkoname];
		else if (is_name(snkoname))
			snkname = snkoname;

		ged = g->get_edge_name(srcname, snkname);
		if (is_variable(lcr)) {
			gcr = g->get_source_rate(ged);
			if (ratevar.find(lcr) == ratevar.end())
				ratevar.insert(make_pair(lcr,gcr));
			else if (ratevar[lcr] != gcr) {
				matching = false;
				return matching;
			}
		}
		if (is_variable(lkr)) {
			gkr = g->get_sink_rate(ged);
			if (ratevar.find(lkr) == ratevar.end())
				ratevar.insert(make_pair(lkr,gkr));
			else if (ratevar[lkr] != gkr) {
				matching = false;
				return matching;
			}
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
	for(auto ac : l->get_actors()) {
		//l->set_solution(ac, g->get_solution(ac));
		r->set_solution(ac, l->get_solution(ac));
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
