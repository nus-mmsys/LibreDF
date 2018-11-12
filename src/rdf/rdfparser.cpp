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

#include "rdfparser.h"

RDFParser::RDFParser() {

}

RDFGraph * RDFParser::get_graph() {
	return rdfg;
}

int RDFParser::load_from_stream(std::stringstream & ss) {
	int ret;
	rdfg = new RDFGraph();
	graph = rdfg->graph;
	string rdfname;
        read_str(ss, "tmf");
        ss >> rdfname;
        rdfg->graph->set_name(rdfname);
	ret = read_graph(ss, rdfg->graph);
	
	if (ret < 0)
		return ret;
	ret = read_rules(ss);
	if (ret < 0)
		return ret;
	ret = read_main(ss);
	if (ret == 0)
		cout << "RDF is loaded successfully.\n";
	else 
		cout << "cannot load the RDF.\n";
	return ret;
}

int RDFParser::read_main(stringstream & stream) {
	int ret;
	ret=read_str(stream, "main");
       	if (ret < 0)
		return ret;

	ret=read_conditions(stream);
	if (ret < 0)
		return ret;

	return 0;
}

int RDFParser::read_conditions(stringstream & stream) {

	int ret=0;
	string condlist, cond;
	read_str(stream, "{");	
	getline(stream, condlist, '}');
	
	trim_str(condlist);

	istringstream ss(condlist);
	while (getline(ss, cond, ';')) {
		ret = add_cond(cond);
		if (ret < 0)
			return ret;		
	}
	return 0;
}

int RDFParser::add_cond(const string& cond) {
	int ret=0, value=0;
	string var="", val="", rule="";
	std::istringstream ss(cond);
  	getline(ss, var, ' ');
	getline(ss, val, ':');
	getline(ss, rule);
        if (var=="" || val=="" || rule=="") {
		cout << "error: condition is not well formatted.\n";
		return -2;
	}
	try {
		value = stoi(val);
	}
	catch(...) {
		cout << "error: value of a condition is not correct.\n";
		return -2;
	}
	//TODO
	//ret = g->add_condition(var,value, rule);
	if (ret == -1)
		cout << "error: condition cannot be added.\n";
	return ret;
}

int RDFParser::read_rules(stringstream & stream) {
	int ret=0;
	string rulename;
	string rulestr;
	Rule * rule;
	rdfg->graph->resolve();
	while (ret >= 0) {
		ret = check_str(stream, "rule");
		if (ret<0)
			break;
		stream >> rulename;
		rule = new Rule(rulename);
		ret = read_str(stream, "{");
		if (ret < 0)
			return ret;
		ret = read_str(stream, "left");
		if (ret < 0)
			return ret;
		ret = read_graph(stream, rule->left());
		if (ret < 0)
			return ret;
		ret = read_str(stream, "right");
		if (ret < 0)
			return ret;
		ret = read_graph(stream, rule->right());
		if (ret < 0)
			return ret;
		ret = read_str(stream, "}");
		if (ret < 0)
			return ret;

		rule->process(rdfg->graph);
	
		rdfg->rules.push_back(rule);		
	}
	return 0;
}
