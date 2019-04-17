/*
 *   TMF
 *   Copyright (C) TMF Team
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

#ifndef DF_PARSER_GRAPH_H
#define DF_PARSER_GRAPH_H

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include "actor.h"
#include "edge.h"

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
	map<string, string> params;
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

	vector<Edge *> get_iedges(Actor * ac);
	vector<Edge *> get_oedges(Actor * ac);

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
	Actor * get_actor(std::string name);
	
	/*!
	 * Get the reference to an edge with a specified name.
	 *
	 * \param name
	 * 	Name of the edge.
	 *
	 * \return
	 * 	Reference to the corresponding edge.
	 *
	 */
	Edge * get_edge(std::string name);

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
	int gcd(int a, int b);
	
	//vector<vector<Actor *>> pathlist;
	//void findpaths();
	//void findpaths_from(Actor * head, vector<Actor *> path);
	//void findpaths2();
public:

  	using actor_iterator = std::map<string, Actor *>::const_iterator;	
  	using edge_iterator = std::map<string, Edge *>::const_iterator;	
	using param_iterator = std::map<string, string>::const_iterator;

	actor_iterator actor_begin() { return actors.begin(); } 
	actor_iterator actor_end() { return actors.end(); } 
	int actor_size() { return actors.size(); } 

	edge_iterator edge_begin() { return edges.begin(); } 
	edge_iterator edge_end() { return edges.end(); } 
	int edge_size() { return edges.size(); } 

	param_iterator param_begin() { return params.begin(); } 
	param_iterator param_end() { return params.end(); } 
	int param_size() { return params.size(); } 


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
	 * Add a parameter to the graph.
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
	int add_graph_param(string key, string val);

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
	int add_actor_prop(string actname, string key, string val);

	/*!
	 * Set properties of an actor of the graph.
	 * 
	 * \param actname
	 * 	Name of the actor.
	 *
	 * \param props
	 * 	The list of all properties.
	 *
	 * \return
	 *
	 */
	int set_actor_props(string actname, std::map<string,string> props);

	/*!
	 * Replace properties of an actor of the graph.
	 * 
	 * \param actname
	 * 	Name of the actor.
	 *
	 * \param props
	 * 	The list of all properties.
	 *
	 * \return
	 *
	 */
        int replace_actor_props(string actname, map<string,string> props);

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
	 * Create a new edge name.
	 * 
	 * \param source
	 * 	Name of the source of the edge.
	 *
	 * \param sink
	 * 	Name of the sink of the edge.
	 *
	 * \return
	 * 	New name = source + sink;
	 *
	 */
	string new_edge_name(string src, string snk);

	/*!
	 * Add an edge to the graph.
	 * 
	 * \param source
	 * 	Name of the source of the edge.
	 *
	 * \param sink
	 * 	Name of the sink of the edge.
	 *
	 * \param src_rate
	 * 	Rate of the source of the edge.
	 *
	 * \param snk_rate
	 * 	Rate of the sink of the edge.
	 *
	 * \return
	 *
	 */
	int add_edge(string edge_source, string edge_sink, int src_rate, int snk_rate, string src_port, string snk_port);
	
	/*!
	 * Returns a list of topological orders of the graph.
	 * 
	 * \return
	 *	List of topological orders.
	 */
	vector<vector<string>> order();

	/*!
	 * Returns a list of hamiltonian paths of the graph.
	 * 
	 * \return
	 *	List of hamiltonian paths.
	 */
	vector<vector<string>> hamiltonians();

	/*!
	 * Returns the adjacency matrix
	 * 
	 * \return
	 *	Adjacency matrix.
	 */
	vector<vector<int>> adjacency();

	/*!
	 * Returns the power N of matrix in Max-Plus algebra
	 * 
	 * \param n
	 * 	Power n
	 * \return
	 *	Adjacency^n in Max-Plus algebra
	 */
	vector<vector<int>> mxpower(int n);
	
	/*!
	 * Performs a DFS to find hamiltonian paths.
	 * 
	 */
	vector<vector<string>> dfs_hamiltonians(Actor * curr, vector<string> stack);

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
	 * Returns the execution time of a given actor.
	 * 
	 * \param name
	 * 	Name of the actor.
	 * 
	 * \return
	 * 	Execution time of the given actor.
	 *
	 */
	int get_exect(string name);

	/*!
	 * Returns the solution of a given actor.
	 * 
	 * \param name
	 * 	Name of the actor.
	 * 
	 * \return
	 * 	Solution of the given actor.
	 *
	 */
	int get_solution(string name);

	/*!
	 * Set the solution for an actor with specified name.
	 * 
	 * \param name
	 * 	Name of the actor.
	 *
	 * \param sol
	 * 	The solution of actor to set.
	 * 
	 * \return
	 *
	 */
	int set_solution(string name, int sol);

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
	 * Get the source rate 
	 * (or parameter if it is parametric)
	 * of a given edge.
	 *
	 * \param edgename
	 * 	Name of the edge.
	 *
	 * \return
	 *	Source rate (or parameter) of the edge.
	 *
	 */
	string get_source_rate_p(string edgename);

	/*!
	 * Get an edge name by the name
	 * of its source and sink actors.
	 *
	 * \param c
	 * 	Name of the source.
	 * \param k
	 *	Name of the sink.
	 *
	 * \return
	 *	Name of the edge.
	 *
	 */
	string get_edge_name(string c, string k);

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
	 * Set the source port for an edge with specified name.
	 * 
	 * \param edgename
	 * 	Name of the edge.
	 *
	 * \param src_port
	 * 	The source port name of the edge to set.
	 *
	 * \return
	 *
	 */
        int set_source_port(string edgename, string src_port);
 
	/*!
	 * Set the sink port for an edge with specified name.
	 * 
	 * \param edgename
	 * 	Name of the edge.
	 *
	 * \param snk_port
	 * 	The sink port name of the edge to set.
	 *
	 * \return
	 *
	 */       
	int set_sink_port(string edgename, string snk_port);

	/*!
	 * Set the parametric source rate for an edge with specified name.
	 * 
	 * \param edgename
	 * 	Name of the edge.
	 *
	 * \param rate
	 * 	The parametric source rate of the edge to set.
	 *
	 * \return
	 *
	 */
	int set_source_param_rate(string edgename, string rate);

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
	 * Get the sink rate 
	 * (or parameter if rate is parametric)
	 * of a given edge.
	 * 
	 * \param edgename
	 * 	Name of the edge.
	 *
	 * \return
	 *	Sink rate (or parameter) of the edge.
	 *
	 */
	string get_sink_rate_p(string edgename);	

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
	 * Set the sink rate for an edge with specified name.
	 * 
	 * \param edgename
	 * 	Name of the edge.
	 *
	 * \param rate
	 * 	The parametric sink rate of the edge to set.
	 *
	 * \return
	 *
	 */
        int set_sink_param_rate(string edgename, string rate);
	
	/*!
	 * Set the initial tokens for an edge with specified name.
	 * 
	 * \param edgename
	 * 	Name of the edge.
	 *
	 * \param tokens
	 * 	The initial tokens of the edge to set.
	 *
	 * \return
	 *
	 */
	int set_initial_tokens(string edgename, int tokens);
	
	/*!
	 * Get the initial tokens of a given edge.
	 * 
	 * \param edgename
	 * 	Name of the edge.
	 *
	 * \return
	 *	Initial tokens of the edge.
	 *
	 */
	int get_initial_tokens(string edgename); 
	
	/*!
	 * Get the source port of a given edge.
	 * 
	 * \param edgename
	 * 	Name of the edge.
	 *
	 * \return
	 *	Source port of the edge.
	 *
	 */
	std::string get_source_port(string edgename);
	
	/*!
	 * Get the sink port of a given edge.
	 * 
	 * \param edgename
	 * 	Name of the edge.
	 *
	 * \return
	 *	Sink port of the edge.
	 *
	 */
	std::string get_sink_port(string edgename);

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
	 * Determines if the graph
	 * contains a given actor.
	 *
	 * \param name
	 * 	The name of the actor.
	 *
	 * \return
	 *	true if the graph contains
	 *	the actor, false otherwise.
	 *
	 */
	bool contains_actor(string name);
	
	/*!
	 * Determines if the graph
	 * contains a given edge.
	 *
	 * \param src
	 * 	The name of the source actor of the edge
	 * \param snk
	 * 	The name of the sink actor of the edge
	 *
	 * \return
	 *	true if the graph contains
	 *	the edge, false otherwise.
	 *
	 */
	bool contains_edge(string src, string snk);

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
	 * Get a property key of a given actor by its value.
	 *
	 * \param actname
	 * 	The name of the actor.
	 *
	 * \param key
	 * 	The property val.
	 *
	 * \return
	 *	The key of property of the actor.
	 *
	 */
	string get_actor_prop_key(string actname, string val);


	/*!
	 * Get the properties of the graph.
	 *
	 * \return
	 *	The map of parameters of the graph.
	 *
	 */
	std::map<std::string, std::string> get_graph_params();

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

	/*!
	 * Compute a schedule for the graph.
	 *
	 * \return
	 * 	A map containing all actors.
	 * 	Each actor has a vector of tuples 
	 * 	for its start times and end times.  
	 *
	 */
	map<string, vector<tuple<int, int>>> schedule();

	/*!
	 * Compute the latency of the graph.
	 * Actors should have their exect property.
	 *
	 * \return
	 * 	The latency of the graph
	 *
	 */
	int latency();
	
	void set_visited(bool v);
	void set_visited(std::string ac, bool v);
	bool get_visited(std::string ac);

	vector<std::string> get_pred(std::string ac);
	vector<std::string> get_succ(std::string ac);

	void reconfigure_from(Graph * g);
	void clear();	
        void print();
	//vector<vector<string>> path();
	//int solve2();

};

#endif
