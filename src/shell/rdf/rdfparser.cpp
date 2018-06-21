/*
 *   RDF
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

#include "rdfparser.h"

RDFParser::RDFParser() {

}

int RDFParser::load_from_file(const char * filename) {
	int ret;
	graph = new Graph();
	string rdfname;
	ifstream file(filename);
        read_str(file, "df");
        file >> rdfname;
        graph->set_name(rdfname);
	ret = read_graph(file, graph);
	if (ret < 0)
		return ret;
	ret = read_rules(file);
	if (ret == -2) {
		cout << "rdf is loaded successfully.\n";
		return 0;
	} else
		return ret;
}

int RDFParser::read_rules(ifstream & file) {
	int ret=0;
	string rulename;
	string rulestr;
	Rule * rule;
	graph->resolve();
	while (!file.eof() && ret >= 0) {
		ret = read_str(file, "rule");
		if (ret < 0)
			return ret;
		file >> rulename;
		rule = new Rule(rulename);
		ret = read_str(file, "{");
		if (ret < 0)
			return ret;
		ret = read_str(file, "left");
		if (ret < 0)
			return ret;
		ret = read_graph(file, rule->left());
		if (ret < 0)
			return ret;
		ret = read_str(file, "right");
		if (ret < 0)
			return ret;
		ret = read_graph(file, rule->right());
		if (ret < 0)
			return ret;
		ret = read_str(file, "}");
		if (ret < 0)
			return ret;

		rule->process(graph);
	
		rules.push_back(rule);		
	}
	return ret;
}


vector<Rule *> RDFParser::get_rules() {
	return rules;
}
