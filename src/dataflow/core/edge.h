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

#ifndef DF_EDGE_H_
#define DF_EDGE_H_

#include "actor.h"
#include "port.h"

namespace df {

    /*!
     * Edge class
     *  A edge has reference to its source and sink ports.
     *
     */
     class Edge {
     private:
	std::string name; /**< Edge name */
	bool visited; /**< Boolean to check the edge is visited in dfs algorithm. */
	Actor * src_actor; /**< Source actor of the edge */
	Actor * snk_actor; /**< Sink actor of the edge */

	IPort * iport; /**< Source port of the edge */
	OPort * oport; /**< Sink port of the edge */

	int src_port_idx; /**< Source port index (to be replaced by iport) */
	int snk_port_idx; /**< Sink port index (to be replaced by oport) */

	std::string src_port; /**< Source port of the edge (t.b.r. by iport) */
	std::string snk_port; /**< Sink port of the edge (t.b.r. by oport) */
	
	int src_rate; /**< Source rate of the edge (t.b.r. by iport) */
	int snk_rate; /**< Sink rate of the edge (t.b.r. by oport) */

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
	Edge(std::string edgename);
	
	/*!
	 * Get name of the edge
	 *
	 * \return
	 * 	Name of the edge
	 *
	 */
	std::string getName();
	
	/*!
	 * Set the source actor of the edge
	 *
	 * \param src
	 * 	The source actor to set.
	 *
	 */
	void setSource(Actor * src);

	/*!
	 * Set the sink actor of the edge
	 *
	 * \param snk
	 * 	The sink actor to set.
	 *
	 */
	void setSink(Actor * snk);
	
	/*!
	 * Set the visited boolean.
	 *
	 * \param v
	 * 	The boolean value to assign to the visited.
	 *
	 */
	void setVisited(bool v);
	
	/*!
	 * Get the visited boolean.
	 *
	 * \return
	 * 	The value of the visited.
	 *
	 */
	bool getVisited();
	
	/*!
	 * Get source actor of the edge.
	 *
	 * \return
	 * 	Source actor of the edge.
	 *
	 */
	Actor * getSource();
	
	/*!
	 * Get sink actor of the edge.
	 *
	 * \return
	 * 	Sink actor of the edge.
	 *
	 */
	Actor * getSink();
	
	/*!
	 * Set source rate of the edge.
	 *
	 * \param rate
	 * 	The source rate to set.
	 *
	 */
	void setSourceRate(int rate);
	
	/*!
	 * Set sink rate of the edge.
	 *
	 * \param rate
	 * 	The sink rate to set.
	 *
	 */
	void setSinkRate(int rate);
	
	std::string readPortName(std::string p, int & idx);

	/*!
	 * Set source port of the edge.
	 *
	 * \param p
	 * 	The source port to set.
	 *
	 */
	void setSourcePort(std::string p);
	std::string getSourcePort();
	int getSourcePortIndex();
	
	/*!
	 * Set sink port of the edge.
	 *
	 * \param p
	 * 	The sink port to set.
	 *
	 */
	void setSinkPort(std::string p);
	std::string getSinkPort();
	int getSinkPortIndex();

	/*!
	 * Get source rate of the edge.
	 *
	 * \return
	 * 	The source rate of the edge.
	 *
	 */
	int getSourceRate();

	/*!
	 * Get sink rate of the edge.
	 *
	 * \return
	 * 	The sink rate of the edge.
	 *
	 */
	int getSinkRate();

	/*!
	 * Get initial tokens of the edge.
	 *
	 * \return
	 * 	The initial tokens of the edge.
	 *
	 */
	int getInitialTokens();

	/*!
	 * Set initial tokens of the edge.
	 *
	 * \param tokens
	 * 	The initial tokens to set.
	 *
	 */
	void setInitialTokens(int tokens);

	/*!
	 * Get current tokens of the edge.
	 *
	 * \return
	 * 	The current tokens of the edge.
	 *
	 */
	int getTokens();

	/*!
	 * Set current tokens of the edge.
	 *
	 * \param t
	 * 	The current tokens to set.
	 *
	 */
	void setTokens(int t);

     };

}

#endif /* DF_EDGE_H_ */
