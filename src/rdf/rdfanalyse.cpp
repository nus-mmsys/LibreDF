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

#include "rdfanalyse.h"

RDFAnalyse::RDFAnalyse(int argc, char * argv[], RDFParser * p) : Analyse(argc,argv,p) {
	cmd["benchmark"] = bind(&RDFAnalyse::display_benchmark, this);
	cmd["rules"] = bind(&RDFAnalyse::display_rules, this);

	comment["benchmark"] = "display benchmark.";
	comment["rules"] = "\tdisplay list of rules.";

	rules = p->get_rules();
}

int RDFAnalyse::display_graph(Graph * g) {
	int ac_index = 0;
	int ed_index = 0;
	vector<string> actorlist = g->get_actors();
	vector<string> edgelist = g->get_edges();
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
	if (ret == -1)
		cout << "error: graph is not consistent.\n";
	else {
		for (auto & ac : actorlist) {
			cout << ac << " : " << g->get_firing(ac) << "\n";
		}
	}
	
	cout << "-------\n";

	cout << "execution times \n\n";
	for (auto & ac : actorlist) {
		cout << ac << " : " << g->get_exect(ac) << "\n";
	}
	
	cout << "-------\n";

	return 0;
}

int RDFAnalyse::display_rules() {

	if (rules.size() == 0) {
		cout << "No rule is found.\n";
		return 0;
	}
	int ret;
	int rule_number=0;
	while (rule_number >= 0 && rule_number<rules.size()) {
	
		for (int i=0; i<rules.size(); i++) {
			cout << to_string(i) << ": " << rules[i]->get_name() << "\n";
		}
		cout << "rules> press q to exit.\n";
		cout << "rules> enter rule number:";
		cin >> rule_number;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(256, '\n');
			return -1;
		}
		if (rule_number <0 || rule_number>=rules.size())
			break;
	
		cout << "=======\n";
		cout << rules[rule_number]->get_name() << " left \n" ;
		cout << "=======\n";
		ret = display_graph(rules[rule_number]->left());
		if (ret < 0)
			return ret;
	
		cout << "=======\n";
		cout << rules[rule_number]->get_name() << " right \n" ;
		cout << "=======\n";
		ret = display_graph(rules[rule_number]->right());
		if (ret < 0)
			return ret;
		cout << "=======\n";
		ret = rules[rule_number]->verify();
		cout << "=======\n";
		if (ret == 0) {
			cout << rules[rule_number]->result()->get_name() << "\n";
			cout << "=======\n";
			ret = display_graph(rules[rule_number]->result());
			if (ret < 0)
				return ret;
			cout << "=======\n";
		}
	}
	return 0;
}

int RDFAnalyse::display_benchmark() {
	if (rules.size() == 0) {
		cout << "No rule is found.\n";
		return 0;
	}
	cout << "name, RDF connectivity, connectivity, RDF consistency, consistency, RDF liveness, liveness\n";
	for (auto r : rules) {
		cout << r->benchmark() << endl;
	}
	return 0;
}
