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

#ifndef DF_PARSER_EDGE_H
#define DF_PARSER_EDGE_H

#include <string>
#include <vector>
#include <map>
#include "actor.h"

using namespace std;

/*!
 * Edge class
 *  A edge has reference to its source and sink ports.
 *
 */
class Edge {
private:
	string name; /**< Edge name */
	bool visited; /**< Boolean to check the edge is visited in dfs algorithm. */
	Port * source; /**< Source port of the edge */
	Port * sink; /**< Sink port of the edge */

	Actor * src_actor;
	Actor * snk_actor;
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
	 * Connect the edge to a source port
	 *
	 * \param sourceport
	 * 	The source port to connect to.
	 *
	 */
	void connect_source(Port * sourceport);
	
	/*!
	 * Connect the edge to a sink port
	 *
	 * \param sinkport
	 * 	The sink port to connect to.
	 *
	 */
	void connect_sink(Port * sinkport);
	
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
	 * Set source rate of the edge.
	 *
	 * \param rate
	 * 	The source rate to set.
	 *
	 */
	void set_source_rate(int rate);
	
	/*!
	 * Set sink rate of the edge.
	 *
	 * \param rate
	 * 	The sink rate to set.
	 *
	 */
	void set_sink_rate(int rate);
	
	/*!
	 * Get source rate of the edge.
	 *
	 * \return
	 * 	The source rate of the edge.
	 *
	 */
	int get_source_rate();

	/*!
	 * Get sink rate of the edge.
	 *
	 * \return
	 * 	The sink rate of the edge.
	 *
	 */
	int get_sink_rate();
};

#endif
