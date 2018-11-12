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
	rdfg = p->get_graph();

	cmd["benchmark"] = bind(&RDFAnalyse::display_benchmark, this);
	cmd["rules"] = bind(&RDFAnalyse::display_rules, this);
	cmd["program"] = bind(&RDFAnalyse::display_prog, this);
	cmd["run"] = bind(&RDFAnalyse::run, this);

	comment["benchmark"] = "display benchmark.";
	comment["rules"] = "\tdisplay list of rules.";
	comment["program"] = "\tdisplay the RDF program.";
	comment["run"] = "\trun the RDF program.";

	cmd.erase("runtcp");
	comment.erase("runtcp");

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

int RDFAnalyse::display_prog() {
	for (auto c : rdfg->prog) {
		for (auto t : c.second) {
			cout << c.first << " "
				<< t.sign << " " << t.val << " : "
			        << t.rule << "\n";
		}
	}
	return 0;
}

int RDFAnalyse::display_rules() {

	if (rdfg->rules.size() == 0) {
		cout << "No rule is found.\n";
		return 0;
	}
	int ret;
	int rule_number=0;
	string rule_name="";
	while (true) {
	
		for (auto r : rdfg->rules) {
			cout << r.first << "\n";
		}
		cout << "rules> press q to exit.\n";
		cout << "rules> enter rule name:";
		cin >> rule_name;
		if (rdfg->rules.find(rule_name)==rdfg->rules.end())
			break;
	
		cout << "=======\n";
		cout << rdfg->rules[rule_name]->get_name() << " left \n" ;
		cout << "=======\n";
		ret = display_graph(rdfg->rules[rule_name]->left());
		if (ret < 0)
			return ret;
	
		cout << "=======\n";
		cout << rdfg->rules[rule_name]->get_name() << " right \n" ;
		cout << "=======\n";
		ret = display_graph(rdfg->rules[rule_name]->right());
		if (ret < 0)
			return ret;
		cout << "=======\n";
		cout << "latency(lhs) = " << rdfg->rules[rule_name]->left()->latency() << "\n";
		cout << "latency(rhs) = " << rdfg->rules[rule_name]->right()->latency() << "\n";
		cout << "\u0394 lat = " << rdfg->rules[rule_name]->right()->latency() -
		       	rdfg->rules[rule_name]->left()->latency() << "\n";
		cout << "=======\n";
		ret = rdfg->rules[rule_name]->verify();
		cout << "=======\n";
		if (ret == 0) {
			cout << rdfg->rules[rule_name]->result()->get_name() << "\n";
			cout << "=======\n";
			ret = display_graph(rdfg->rules[rule_name]->result());
			if (ret < 0)
				return ret;
			cout << "=======\n";
		}
	}
	return 0;
}

int RDFAnalyse::run() {

	df::Dataflow * dfg = parser->get_dataflow();

	dfg->init();

	dfg->connect();

  	dfg->run();

	delete dfg;

	return 0;
}

int RDFAnalyse::display_benchmark() {
	if (rdfg->rules.size() == 0) {
		cout << "No rule is found.\n";
		return 0;
	}
	cout << "name, RDF connectivity, connectivity, RDF consistency, consistency, RDF liveness, liveness\n";
	for (auto r : rdfg->rules) {
		cout << r.second->benchmark() << endl;
	}
	return 0;
}
