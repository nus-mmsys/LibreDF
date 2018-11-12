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

int RDFParser::load_from_stream(std::stringstream & ss) {
	int ret;
	graph = new Graph();
	string rdfname;
        read_str(ss, "tmf");
        ss >> rdfname;
        graph->set_name(rdfname);
	ret = read_graph(ss, graph);
	if (ret < 0)
		return ret;
	ret = read_rules(ss, graph);
	if (ret < 0)
		return ret;
	ret = read_main(ss, graph);
	if (ret == 0)
		cout << "RDF is loaded successfully.\n";
	else 
		cout << "cannot load the RDF.\n";
	return ret;
}

int RDFParser::read_main(stringstream & stream, Graph * g) {
	int ret;
	ret=read_str(stream, "main");
       	if (ret < 0)
		return ret;

	ret=read_str(stream, "{");
       	if (ret < 0)
		return ret;

	ret=read_str(stream, "}");
       	if (ret < 0)
		return ret;

	return 0;
}

int RDFParser::read_rules(stringstream & stream, Graph * g) {
	int ret=0;
	string rulename;
	string rulestr;
	Rule * rule;
	g->resolve();
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

		rule->process(g);
	
		rules.push_back(rule);		
	}
	return 0;
}

vector<Rule *> RDFParser::get_rules() {
	return rules;
}
