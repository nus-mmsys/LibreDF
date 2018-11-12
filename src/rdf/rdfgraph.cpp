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

#include "rdfgraph.h"

RDFGraph::RDFGraph() {
	graph = new Graph();
}

int RDFGraph::add_rule(Rule * r) {
	r->process(graph);
	rules.insert(make_pair(r->get_name(), r));
	return 0;
}
int RDFGraph::add_condition(string var, char sign, int val, string rule) {
	if (rules.find(rule) == rules.end()) {
		cerr << "condition applies an unknown rule.\n";
		return -1;
	}

	Condition cond;
	cond.set(var, sign, val, rule);
	
	if (prog.find(var) == prog.end()) {
		vector<Condition> v;
		v.push_back(cond);
		prog.insert(make_pair(var,v));	
	} else {
		prog[var].push_back(cond);
	}

	return 0;
}

void Condition::set(string vr, char sn, int vl, string rl) {
		var = vr;
		sign = sn;
		val = vl;
		rule = rl;
}
