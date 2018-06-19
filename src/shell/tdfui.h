/*
 *   DF
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

#ifndef DF_TTDFUI_H
#define DF_TTDFUI_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <functional>
#include "graph.h"
#include "core/df.h"

using namespace std;

/*!
 * \class TDFUI
 * The user interface class for RDF.
 *
 */
class TDFUI {

private:
	Graph * graph; /**< RDF data graph. */

	map<string, function<int()>> cmd; /**< A map from command name to command funtion. */
	map<string, string> comment; /**< A map from command name to command description. */

	/*! 
	 * Load an RDF from file.
	 *
	 * \param filename
	 * 	Name of the file in string format.
	 *
	 * \return 
	 *
	 */
	int load_from_file(const char * filename);
	
	/*!
	 * Read a string from a file stream.
	 *
	 * \param file
	 * 	Reference of the file stream.
	 *
	 * \param str
	 * 	String to read from the file stream.
	 *
	 * \return
	 *
	 */ 
	int read_str(ifstream & file, string str);

	/*!
	 * Read a topology from a file stream.
	 *
	 * \param file
	 * 	Reference of the file stream.
	 *
	 * \param g
	 * 	Reference of graph to read from the file stream.
	 *
	 * \return
	 *
	 */ 
	int read_topology(ifstream & file, Graph * g);

	/*!
	 * Read a graph from a file stream.
	 *
	 * \param file
	 * 	Reference of the file stream.
	 *
	 * \param g
	 * 	Reference of graph to read from the file stream.
	 *
	 * \return
	 *
	 */ 
	int read_graph(ifstream & file, Graph * g);

	/*!
	 * Read the list of actors of a graph from a file stream.
	 *
	 * \param file
	 * 	Reference of the file stream.
	 *
	 * \param g
	 * 	Reference of graph containing the actors to read from the file stream.
	 *
	 * \return
	 *
	 */ 
	int read_actors(ifstream & file, Graph * g);
	
	/*!
	 * Read the production rates from a file stream.
	 *
	 * \param file
	 * 	Reference of the file stream.
	 * 
	 * \param g
	 * 	Reference of graph containing the actor
	 *
	 * \return
	 *
	 */ 
        int read_productions(ifstream & file, Graph * g);

	/*!
	 * Read the consumption rates from a file stream.
	 *
	 * \param file
	 * 	Reference of the file stream.
	 * 
	 * \param g
	 * 	Reference of graph containing the actor
	 *
	 * \return
	 *
	 */ 
        int read_consumptions(ifstream & file, Graph * g);


	/*!
	 * Read the list of properties of an actor from 
	 * a file stream.
	 *
	 * \param file
	 * 	Reference of the file stream.
	 * 
	 * \param g
	 * 	Reference of graph containing the actor
	 *
	 * \return
	 *
	 */ 
        int read_props(ifstream & file, Graph * g);

	/*!
	 * Load the actor type from the property
	 * "computation"
	 *
	 * \param g
	 * 	Reference of graph containing the actor
	 *
	 * \return
	 *
	 */ 
        int load_actor_types(Graph * g);

	/*!
	 * Read the list of edges of a graph from a file stream.
	 *
	 * \param file
	 * 	Reference of the file stream.
	 *
	 * \param g
	 * 	Reference of graph containing the edges to read from the file stream.
	 *
	 * \return
	 *
	 */ 
	int read_edges(ifstream & file, Graph * g);

	/*!
	 * Add production rate.
	 *
	 * \param actor
	 * 	A string containing the name of an actor specified rate
	 * 	edge_name = edge_rate
	 *
	 * \param g
	 * 	Reference of graph to which the actor is added.
	 *
	 * \return
	 *
	 */ 
	int add_production_rate(const string& rate, Graph * g);

	/*!
	 * Add consumption rate.
	 *
	 * \param actor
	 * 	A string containing the name of an actor specified rate
	 * 	edge_name = edge_rate
	 *
	 * \param g
	 * 	Reference of graph to which the actor is added.
	 *
	 * \return
	 *
	 */ 
	int add_consumption_rate(const string& rate, Graph * g);


	/*!
	 * Add an actor to a graph.
	 *
	 * \param actor
	 * 	A string containing the name of an actor specified name:type
	 *
	 * \param g
	 * 	Reference of graph to which the actor is added.
	 *
	 * \return
	 *
	 */ 
	int add_actor(const string& actor, Graph * g);

	/*!
	 * Add an actor specified with a type to a graph.
	 *
	 * \param actortype
	 * 	A string containing the name and type of an actor specified name:type
	 *
	 * \param g
	 * 	Reference of graph to which the actor is added.
	 *
	 * \return
	 *
	 */ 
	int add_actortype(const string& actortype, Graph * g);
	
	/*!
	 * Add a property of an actor to a graph.
	 *
	 * \param actname
	 * 	The name of the actor.
	 *
	 * \param prop
	 *      The property to set.
	 *
	 * \param g
	 * 	Reference of graph containing the actor.
	 *
	 * \return
	 *
	 */ 
        int add_prop(const string& actname, const string& prop, Graph * g);
	
	/*!
	 * Add an edge specified with its source and sink actors to a graph.
	 *
	 * \param edge_source
	 * 	A string ntaining the name of the source actor of the edge.
	 *
	 * \param edge_sink
	 * 	A string ntaining the name of the sink actor of the edge.
	 *
	 * \param g
	 * 	Reference of graph to which the edge is added.
	 *
	 * \return
	 *
	 */ 
	int add_edge(string edge_source, string edge_sink, Graph * g);
	
	/*!
	 * Add an edge specified with its source and sink actors to a graph.
	 *
	 * \param edge
	 * 	A string containing
	 * 	"edge_name (edge_source, edge_sink)"
	 *
	 * \param g
	 * 	Reference of graph to which the edge is added.
	 *
	 * \return
	 *
	 */ 
	int add_edge(const string& edge, Graph * g);

	/*!
	 * Display a graph to the user
	 *
	 * \param g
	 * 	Reference to the graph to display
	 *
	 * \return
	 *
	 */ 
	int display_graph(Graph * g);

	/*!
	 * Display the RDF graph to the user
	 *
	 * \return
	 *
	 */ 
	int display_rdf_graph();

	/*!
	 * Display the help menu to the user
	 *
	 * \return
	 *
	 */ 
	int display_help();

	/*!
	 * Run the data graph
	 *
	 * \return
	 *
	 */ 
	int run_graph();

	/*!
	 * Process a command given by the user
	 *
	 *  \param command
	 * 	A string containing the command
	 *
	 * \return
	 *
	 */ 
	int process_command(string command);

	//int edges();	
	//int rates();
	//int firings();

public:

	/*!
	 * TDFUI constructor
	 *
	 *  \param argc
	 * 	The number of arguments
	 *
	 *  \param argv
	 *  	The list of strings containing the arguments
	 *
	 */ 
	TDFUI(int argc, char * argv[]);
	
	/*!
	 * Main loop of the TDFUI
	 *
	 * \return
	 *
	 */ 
	int loop();
};

#endif
