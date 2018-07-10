/*
 *   TMF
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

#ifndef DF_PARSER_H
#define DF_PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <functional>
#include "graph.h"
#include "dataflow/core/df.h"

using namespace std;

/*!
 * \class Parser
 *
 */
class Parser {

protected:
	Graph * graph; /**< data graph. */

	std::string dfstr; /**< content of df file in string format. */
	std::map<std::string, std::string> dfactor; /**< map from actor names to actor descriptions in df file. */
	std::map<std::string, vector<std::string>> iplookup; /**< map from ips to actor names. */
	std::string topology; /**< topology of the df file. */
	std::string parameter; /**< parameter section of the df file. */

	/*!
	 * Trims a string and removes all 
	 * ' ', '\t', and '\n'.
	 *
	 * \param str
	 * 	String to trim.
	 *
	 * \return
	 *
	 */ 
	int trim_str(string & str);

	/*!
	 * Insert an IP and its corresponding actor name
	 * into iplookup table.
	 *
	 * \param ip
	 * 	IP address
	 * \param actname
	 * 	Actor name
	 *
	 */ 
	void insert_lookup(std::string ip, std::string actname);

	/*!
	 * Read a string from a stream.
	 *
	 * \param stream
	 * 	Reference of the stream.
	 *
	 * \param str
	 * 	String to read from the stream.
	 *
	 * \return
	 *
	 */ 
	int read_str(std::stringstream & stream, string str);

	/*!
	 * Read a topology from a stream.
	 *
	 * \param stream
	 * 	Reference of the stream.
	 *
	 * \param g
	 * 	Reference of graph to read from the stream.
	 *
	 * \return
	 *
	 */ 
	int read_topology(std::stringstream & stream, Graph * g);

	/*!
	 * Read a graph from a stream.
	 *
	 * \param stream
	 * 	Reference of the stream.
	 *
	 * \param g
	 * 	Reference of graph to read from the stream.
	 *
	 * \return
	 *
	 */ 
	int read_graph(std::stringstream & stream, Graph * g);

	/*!
	 * Read the list of actors of a graph from a stream.
	 *
	 * \param stream
	 * 	Reference of the stream.
	 *
	 * \param g
	 * 	Reference of graph containing the actors to read from the stream.
	 *
	 * \return
	 *
	 */ 
	int read_actors(std::stringstream & stream, Graph * g);
	
	/*!
	 * Read the production rates from a stream.
	 *
	 * \param stream
	 * 	Reference of the stream.
	 * 
	 * \param g
	 * 	Reference of graph containing the actor
	 *
	 * \return
	 *
	 */ 
        int read_productions(std::stringstream & stream, Graph * g);

	/*!
	 * Read the consumption rates from a stream.
	 *
	 * \param stream
	 * 	Reference of the stream.
	 * 
	 * \param g
	 * 	Reference of graph containing the actor
	 *
	 * \return
	 *
	 */ 
        int read_consumptions(std::stringstream & stream, Graph * g);
	
	/*!
	 * Read the list of parameters of the dataflow
	 * from a stream.
	 *
	 * \param stream
	 * 	Reference of the stream.
	 * 
	 * \param g
	 * 	Reference of graph containing the actor
	 *
	 * \return
	 *
	 */ 
        int read_parameters(std::stringstream & stream, Graph * g);

	/*!
	 * Read the list of properties of an actor from 
	 * a stream.
	 *
	 * \param stream
	 * 	Reference of the stream.
	 * 
	 * \param g
	 * 	Reference of graph containing the actor
	 *
	 * \return
	 *
	 */ 
        int read_props(std::stringstream & stream, Graph * g);

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
	 * Read the list of edges of a graph from a stream.
	 *
	 * \param stream
	 * 	Reference of the stream.
	 *
	 * \param g
	 * 	Reference of graph containing the edges to read from the stream.
	 *
	 * \return
	 *
	 */ 
	int read_edges(std::stringstream & stream, Graph * g);

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
	 * Add a property to the graph.
	 *
	 * \param param
	 *      The parameter to set.
	 *
	 * \param g
	 * 	Reference of graph containing the actor.
	 *
	 * \return
	 *
	 */ 
	int add_graph_param(const string& param, Graph * g);

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
        int add_actor_prop(const string& actname, const string& prop, Graph * g);
	
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

	virtual int load_from_stream(std::stringstream& ss);
public:

	/*!
	 * Parser constructor
	 */ 
	Parser();

	/*! 
	 * Load a graph from file.
	 *
	 * \param filename
	 * 	Name of the file in string format.
	 *
	 * \return 
	 *
	 */
	int load_from_file(const char * filename);

	/*! 
	 * Load a graph from string.
	 *
	 * \param app
	 * 	Application in string format.
	 *
	 * \return 
	 *
	 */
	int load_from_string(const std::string& app);

	std::string get_df_string();

	Graph * get_graph();	
};

#endif
