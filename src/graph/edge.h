/*
 *   LibreDF
 *   Copyright (C) A. Shafiei
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

#ifndef DF_PARSER_EDGE_H
#define DF_PARSER_EDGE_H

#include <string>
#include <vector>
#include <map>
#include "actor.h"

using namespace std;

/*!
 * Rate class
 *  A rate keeps its symbol (if necessary) and an
 *  integer value.
 *
 */
class Rate {
private:
	string symbol;
	int val;
public:
	Rate();
	Rate(int v);
	Rate(string s, int v);
	void set_symbol(string s);
	string get_symbol();
	void set_value(int v);
	int get_value();
};

/*!
 * Edge class
 *  A edge has reference to its source and sink ports.
 *
 */
class Edge {
private:
	string name; /**< Edge name */
	bool visited; /**< Boolean to check the edge is visited in dfs algorithm. */
	Actor * src_actor;
	Actor * snk_actor;

	string src_port; /**< Source port of the edge */
	string snk_port; /**< Sink port of the edge */

	Rate src_rate;
	Rate snk_rate;

	int init_tokens;
	int tokens;

public:
	/*!
	 * Edge constructor
	 *
	 */
	Edge();
	
	/*!
	 * Edge constructor with name
	 *
	 * \param edgename
	 * 	Name of the edge
	 *
	 */
	Edge(string edgename);
	
	/*!
	 * Get name of the edge
	 *
	 * \return
	 * 	Name of the edge
	 *
	 */
	string get_name();
	
	
	/*!
	 * Set the visited boolean.
	 *
	 * \param v
	 * 	The boolean value to assign to the visited.
	 *
	 */
	void set_visited(bool v);
	
	/*!
	 * Get the visited boolean.
	 *
	 * \return
	 * 	The value of the visited.
	 *
	 */
	bool get_visited();
	
	/*!
	 * Get source actor of the edge.
	 *
	 * \return
	 * 	Source actor of the edge.
	 *
	 */
	Actor * get_source_actor();
	
	/*!
	 * Get sink actor of the edge.
	 *
	 * \return
	 * 	Sink actor of the edge.
	 *
	 */
	Actor * get_sink_actor();
	
	void set_source_actor(Actor * src);
	void set_sink_actor(Actor * snk);
	
	/*!
	 * Set the source port of the edge
	 *
	 * \param src
	 * 	The source port name.
	 *
	 */
	void set_source_port(std::string src);
	
	/*!
	 * Set the sink port of the edge
	 *
	 * \param snk
	 * 	The sink port name.
	 *
	 */
	void set_sink_port(std::string snk);
	
	std::string get_source_port();
	std::string get_sink_port();

	/*!
	 * Set source rate of the edge.
	 *
	 * \param rate
	 * 	The source rate to set.
	 *
	 */
	void set_source_rate(int rate);
	
	/*!
	 * Set parametric source rate of the edge.
	 *
	 * \param rate
	 * 	The parametric source rate to set.
	 *
	 */
	void set_source_param_rate(string rate);

	/*!
	 * Set sink rate of the edge.
	 *
	 * \param rate
	 * 	The sink rate to set.
	 *
	 */
	void set_sink_rate(int rate);
	
	/*!
	 * Set parametric sink rate of the edge.
	 *
	 * \param rate
	 * 	The parametric sink rate to set.
	 *
	 */
	void set_sink_param_rate(string rate);

	/*!
	 * Get source rate of the edge.
	 *
	 * \return
	 * 	The source rate of the edge.
	 *
	 */
	int get_source_rate();

	/*!
	 * Get source rate (or parameter) of the edge.
	 *
	 * \return
	 * 	The source rate (or parameter) of the edge.
	 *
	 */
	string get_source_rate_p();

	/*!
	 * Get sink rate of the edge.
	 *
	 * \return
	 * 	The sink rate of the edge.
	 *
	 */
	int get_sink_rate();

	/*!
	 * Get sink rate (or parameter) of the edge.
	 *
	 * \return
	 * 	The sink rate (or parameter) of the edge.
	 *
	 */
	string get_sink_rate_p();

	/*!
	 * Set intial tokens of the edge.
	 *
	 * \param tokens
	 * 	The initial tokens to set.
	 *
	 */
	void set_init_tokens(int tokens);

	/*!
	 * Get initial tokens of the edge.
	 *
	 * \return
	 * 	The initial tokens of the edge.
	 *
	 */
	int get_init_tokens();

	/*!
	 * Set current tokens of the edge.
	 *
	 * \param t
	 * 	The current tokens to set.
	 *
	 */
	void set_tokens(int t);

	/*!
	 * Get current tokens of the edge.
	 *
	 * \return
	 * 	The current tokens of the edge.
	 *
	 */
	int get_tokens();


};

#endif
