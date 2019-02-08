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

#ifndef RDFAnalyse_H
#define RDFAnalyse_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <functional>
#include "analyse/analyse.h"
#include "rdf/rdfparser.h"

using namespace std;

/*!
 * \class RDFAnalyse
 * The user interface class for RDF.
 *
 */
class RDFAnalyse : public Analyse {

private:
	RDFGraph * rdfg;
	RDFParser * rdfparser;
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
	 * Display the RDF program
	 *
	 * \return
	 *
	 */ 
	int display_prog();

	/*!
	 * Display the benchmark to the user
	 *
	 * \return
	 *
	 */ 
	int display_benchmark();

	/*!
	 * Run the RDF graph
	 *
	 * \return
	 *
	 */ 
	int run();

public:
	/*!
	 * RDFAnalyse constructor
	 *
	 *  \param argc
	 * 	The number of arguments
	 *
	 *  \param argv
	 *  	The list of strings containing the arguments
	 *
	 */ 
	RDFAnalyse(int argc, char * argv[], RDFParser * p);
};

#endif
