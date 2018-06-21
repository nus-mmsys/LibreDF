/*
 *   TDF
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

#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "actor.h"

using namespace std;

/*!
 * \class Graph
 *	RDF graph
 */
class Graph {

private:
        string name; /**< Graph name */
	map<string, Actor *> actors; /**< A map from names of actors to the reference of the actors. */
	map<string, Edge *> edges; /**< A map from names of edges to the reference of the edges. */

	/*!
	 * Create an actor with specified name and type in the graph.
	 *
	 * \param name
	 * 	Name of the actor
	 *
	 * \param type
	 * 	Type of the actor
	 *
	 * \return
	 * 	Reference to the actor
	 *
	 */
	Actor * create_actor(string name, string type);

	/*!
	 * Create an edge with an specified name in the graph.
	 *
	 * \param name
	 * 	Name of the edge
	 *
	 * \return
	 * 	Reference to the edge
	 *
	 */
	Edge * create_edge(string name);

	/*!
	 * Find the sources (actors with no incoming edge) of the graph.
	 *
	 * \return
	 * 	Reference to a list of the references of actors.
	 *
	 */
	vector<Actor *> findsources();

	/*!
	 * Takes a list of actors and returns a list of orders of these actors.
	 *
	 * \param heads
	 * 	List of actors.
	 *
	 * \return
	 * 	Reference a list of different orders.
	 *
	 */
	vector<vector<Actor *>> combinations(vector<Actor *> heads);

	/*!
	 * Find topological orders of actors from a specified list of traversed actors
	 *
	 * \param heads
	 * 	List of traversed actors.
	 *
	 * \return
	 * 	A list of orders.
	 *
	 */
	vector<vector<Actor *>> findorders_from(vector<Actor *> heads);

	/*!
	 * Find topological orders of a the graph.
	 *
	 * \return
	 * 	A list of orders.
	 *
	 */
	vector<vector<Actor *>> findorders();

	/*!
	 * A DFS (Depth First Search) to solve the graph for consistency with fractional values.
	 *
	 * \param curr
	 * 	Current actor to visit in the recursive function
	 *
	 * \param num
	 * 	Numinator of the fraction of the solution of the current actor.
	 *
	 * \param den
	 * 	Denuminator of the fraction of the solution of the current actor.
	 *
	 * \return
	 *
	 */
	int dfs(Actor * curr, int num, int den);

	/*!
	 * A DFS (Depth First Search) to visit all actors to determine connectivity.
	 *
	 * \param curr
	 * 	Current actor to visit in the recursive function
	 *
	 * \param visited
	 * 	An integer to indicate how many actors are visited.
	 *
	 */
	void dfs_visited_actors(Actor * curr, int & visited);
	
	/*!
	 * Get the edge with specified source and sink actors.
	 *
	 * \param source
	 * 	Reference to the source actor.
	 *
	 * \param sink
	 * 	Reference to the sink actor.
	 *
	 * \return
	 * 	Reference to the corresponding edge.
	 *
	 */
	Edge * get_edge(Actor * source, Actor * sink);
	
	/*!
	 * Get the reference to an actor with a specified name.
	 *
	 * \param name
	 * 	Name of the actor.
	 *
	 * \return
	 * 	Reference to the corresponding actor.
	 *
	 */
	Actor * get_actor(string name);
	
	/*!
	 * GCD (Greatest Common Divisor) of two integers.
	 *
	 * \param a
	 * 	The first integer.
	 *
	 * \param b
	 * 	The second integer.
	 *
	 * \return
	 * 	The GCD of a and b.
	 *
	 */
	int gdc(int a, int b);
	
	//vector<vector<Actor *>> pathlist;
	//void findpaths();
	//void findpaths_from(Actor * head, vector<Actor *> path);
	//void findpaths2();
public:
      
	/*!
	 * Set the name of the graph.
	 *
	 * \param name
	 * 	The name of the graph to set.
	 *
	 */
      	void set_name(string name);
  
	/*!
	 * Get the name of the graph.
	 *
	 * \return
	 * 	The name of the graph.
	 *
	 */
	string get_name();
 
	/*!
	 * Returns the number of the edges of the graph.
	 *
	 * \return
	 * 	The number of the edges.
	 *
	 */
	int edge_count() { return edges.size(); }
 
	/*!
	 * Add an actor to the graph.
	 * 
	 * \param name
	 * 	Name of the actor to add.
	 *
	 * \param type
	 * 	Type of the actor to add.
	 *
	 * \return
	 *
	 */
	int add_actor(string name, string type);

	/*!
	 * Add a property of an actor of the graph.
	 * 
	 * \param actname
	 * 	Name of the actor.
	 *
	 * \param key
	 * 	The key of the property.
	 *
	 * \param val
	 *      The value of the property.
	 *
	 * \return
	 *
	 */
	int add_prop(string actname, string key, string val);

	/*!
	 * Add an edge to the graph.
	 * 
	 * \param edgename
	 * 	Name of the edge.
	 *
	 * \param source
	 * 	Name of the source of the edge.
	 *
	 * \param sink
	 * 	Name of the sink of the edge.
	 *
	 * \return
	 *
	 */
	int add_edge(string edgename, string source, string sink);

	/*!
	 * Add an edge to the graph.
	 * 
	 * \param source
	 * 	Name of the source of the edge.
	 *
	 * \param sink
	 * 	Name of the sink of the edge.
	 *
	 * \return
	 *
	 */
	int add_edge(string source, string sink);

	/*!
	 * Returns a list of topological orders of the graph.
	 * 
	 * \return
	 *	List of topological orders.
	 */
	vector<vector<string>> order();

	/*!
	 * Returns a list of SAS (Single Appearance Schedules) of the graph.
	 * 
	 * \return
	 *	List of topological SAS.
	 */
	vector<string> sas();
	
	/*!
	 * Solve the graph given a number of solutions of actors.
	 * 
	 * \return
	 *
	 */
	int solve();
	
	/*!
	 * Resolve the graph by setting the solutions to zero at the start.
	 * 
	 * \return
	 *
	 */
	int resolve();
	
	/*!
	 * Returns of the graph is connected.
	 * 
	 * \return
	 * 	True if connected, false otherwise.
	 *
	 */
	bool connected();

	/*!
	 * Returns the firing (solution) of a given actor.
	 * 
	 * \param name
	 * 	Name of the actor.
	 * 
	 * \return
	 * 	Solution of the given actor.
	 *
	 */
	int get_firing(string name);

	/*!
	 * Set the firing (solution) for an actor with specified name.
	 * 
	 * \param name
	 * 	Name of the actor.
	 *
	 * \param firing
	 * 	The solution of actor to set.
	 * 
	 * \return
	 *
	 */
	int set_firing(string name, int firing);

	/*!
	 * Get the source rate of a given edge.
	 * 
	 * \param edgename
	 * 	Name of the edge.
	 *
	 * \return
	 *	Source rate of the edge.
	 *
	 */
	int get_source_rate(string edgename);

	/*!
	 * Set the source rate for an edge with specified name.
	 * 
	 * \param edgename
	 * 	Name of the edge.
	 *
	 * \param rate
	 * 	The source rate of the edge to set.
	 *
	 * \return
	 *
	 */
	int set_source_rate(string edgename, int rate);

	/*!
	 * Get the sink rate of a given edge.
	 * 
	 * \param edgename
	 * 	Name of the edge.
	 *
	 * \return
	 *	Sink rate of the edge.
	 *
	 */
	int get_sink_rate(string edgename);	

	/*!
	 * Set the sink rate for an edge with specified name.
	 * 
	 * \param edgename
	 * 	Name of the edge.
	 *
	 * \param rate
	 * 	The sink rate of the edge to set.
	 *
	 * \return
	 *
	 */
	int set_sink_rate(string edgename, int rate);

	/*!
	 * Get the source name of a given edge.
	 * 
	 * \param edgename
	 * 	Name of the edge.
	 *
	 * \return
	 *	Source name of the edge.
	 *
	 */
	string get_source_name(string edgename);

	/*!
	 * Get the source type of a given edge.
	 * 
	 * \param edgename
	 * 	Name of the edge.
	 *
	 * \return
	 *	Source type of the edge.
	 *
	 */
	string get_source_type(string edgename);

	/*!
	 * Get the sink name of a given edge.
	 * 
	 * \param edgename
	 * 	Name of the edge.
	 *
	 * \return
	 *	Sink name of the edge.
	 *
	 */
	string get_sink_name(string edgename);	

	/*!
	 * Get the sink type of a given edge.
	 * 
	 * \param edgename
	 * 	Name of the edge.
	 *
	 * \return
	 *	Sink type of the edge.
	 *
	 */
	string get_sink_type(string edgename);	

	/*!
	 * Get the list of all actors.
	 * 
	 * \return
	 *	List of the name of the actors.
	 *
	 */
	vector<string> get_actors();

	/*!
	 * Get the list of all edges.
	 * 
	 * \return
	 *	List of the name of the edges.
	 *
	 */
	vector<string> get_edges();

	/*!
	 * Set the type of a given actors.
	 *
	 * \param name
	 * 	The name of the actor.
	 * \param type
	 * 	The type of the actor.
	 *
	 * \return
	 *
	 */
	int set_actor_type(string name, string type);


	/*!
	 * Get the type of a given actors.
	 *
	 * \param name
	 * 	The name of the actor.
	 *
	 * \return
	 *	The type of the actor.
	 *
	 */
	string get_actor_type(string name);

	/*!
	 * Get a property of a given actor.
	 *
	 * \param actname
	 * 	The name of the actor.
	 *
	 * \param prop
	 * 	The property to retrieve.
	 *
	 * \return
	 *	The value of property of the actor.
	 *
	 */
	string get_actor_prop(string actname, string prop);

	/*!
	 * Get the properties of a given actor.
	 *
	 * \param name
	 * 	The name of the actor.
	 *
	 * \return
	 *	The map of properties of the actor.
	 *
	 */
	std::map<std::string, std::string> get_actor_props(string actname);

	/*!
	 * Determine if the graph is empty.
	 *
	 * \return
	 * 	True if empty, false otherwise.
	 *
	 */
	bool empty();

	//vector<vector<string>> path();
	//int solve2();

};

#endif
