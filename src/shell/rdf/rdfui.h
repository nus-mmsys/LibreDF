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

#ifndef RDFUI_H
#define RDFUI_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <functional>
#include "rdf/rdfui.h"
#include "rdf/rdfparser.h"
#include "ui/ui.h"
#include "core/df.h"

using namespace std;

/*!
 * \class RDFUI
 * The user interface class for RDF.
 *
 */
class RDFUI : public UI {

private:
	vector<Rule *> rules;
protected:

	/*!
	 * Display a graph to the user
	 *
	 * \param g
	 * 	Reference to the graph to display
	 *
	 * \return
	 *
	 */ 
	virtual int display_graph(Graph * g);

	/*!
	 * Display the list of rules to the user
	 *
	 * \return
	 *
	 */ 
	int display_rules();

	/*!
	 * Display the benchmark to the user
	 *
	 * \return
	 *
	 */ 
	int display_benchmark();


public:
	/*!
	 * RDFUI constructor
	 *
	 *  \param argc
	 * 	The number of arguments
	 *
	 *  \param argv
	 *  	The list of strings containing the arguments
	 *
	 */ 
	RDFUI(int argc, char * argv[], RDFParser * p);
};

#endif
