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

	/*!
	 * Read a set of rules from a file stream.
	 *
	 * \param file
	 * 	Reference of the file stream.
	 *
	 * \return
	 *
	 */ 
	int read_rules(ifstream & file);

public:

	/*!
	 * RDFParser constructor
	 *
	 */ 
	RDFParser();

	virtual int load_from_file(const char * filename);

	vector<Rule *> get_rules();

};

#endif
