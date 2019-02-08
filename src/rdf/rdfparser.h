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
#include "rdfgraph.h"
#include "rdataflow.h"

using namespace std;

/*!
 * \class RDFParser
 * The parser class for RDF.
 *
 */
class RDFParser : public Parser {
private:
	RDFGraph * rdfg;
protected:
	/*!
	 * Read a set of rules from a stream.
	 *
	 * \param stream
	 * 	Reference of the stream.
	 *
	 * \return
	 *
	 */ 
	int read_rules(std::stringstream & stream);

	/*!
	 * Read the main.
	 *
	 * \param stream
	 * 	Reference of the stream.
	 *
	 * \return
	 *
	 */ 
	int read_main(stringstream & stream);

	/*!
	 * Read the conditions of the main
	 * in the format of
	 * 	variable value : rule;
	 * e.g.
	 * 	iter 32 : r1;
	 *
	 * \param stream
	 * 	Reference of the stream.
	 *
	 * \return
	 *
	 */ 
	int read_conditions(stringstream & stream);

	/*!
	 *
	 * Add a condition of the main
	 * to the RDF graph structure
	 *
	 * \param stream
	 * 	Reference of the stream.
	 *
	 * \return
	 *
	 */ 
	int add_cond(const string& cond);
public:

	/*!
	 * RDFParser constructor
	 *
	 */ 
	RDFParser();

	virtual int load_from_stream(std::stringstream& ss);

	RDFGraph * get_graph();

	RDataflow * get_rdataflow();
};

#endif
