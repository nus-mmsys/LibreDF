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

#ifndef RDFPARSER_H
#define RDFPARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <functional>
#include "parser/parser.h"
#include "rule.h"

using namespace std;

/*!
 * \class RDFParser
 * The parser class for RDF.
 *
 */
class RDFParser : public Parser {

protected:
	vector<Rule *> rules; /**< RDF transformation rules. */
	map<string, vector<tuple<int,string>>> prog; /**< RDF program containing the values of variables for which the rules are applied. */

	/*!
	 * Read a set of rules from a stream.
	 *
	 * \param stream
	 * 	Reference of the stream.
	 * \param g
	 * 	Reference of the graph.
	 *
	 * \return
	 *
	 */ 
	int read_rules(std::stringstream & stream, Graph * g);

	/*!
	 * Read the main.
	 *
	 * \param stream
	 * 	Reference of the stream.
	 * \param g
	 * 	Reference of the graph.
	 *
	 * \return
	 *
	 */ 
	int read_main(stringstream & stream, Graph * g);

	/*!
	 * Read the conditions of the main
	 * in the format of
	 * 	variable value : rule;
	 * e.g.
	 * 	iter 32 : r1;
	 *
	 * \param stream
	 * 	Reference of the stream.
	 * \param g
	 * 	Reference of the graph.
	 *
	 * \return
	 *
	 */ 
	int read_conditions(stringstream & stream, Graph * g);

	/*!
	 *
	 * Add a condition of the main
	 * to the RDF graph structure
	 *
	 * \param stream
	 * 	Reference of the stream.
	 * \param g
	 * 	Reference of the graph.
	 *
	 * \return
	 *
	 */ 
	int add_cond(const string& cond, Graph * g);
public:

	/*!
	 * RDFParser constructor
	 *
	 */ 
	RDFParser();

	virtual int load_from_stream(std::stringstream& ss);

	vector<Rule *> get_rules();

};

#endif
