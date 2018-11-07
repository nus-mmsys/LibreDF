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

#ifndef RDF_RULE_H
#define RDF_RULE_H

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <time.h>
#include "graph/graph.h"

using namespace std;

/*!
 * \class Rule
 * The Rule class for RDF transformation rule.
 *
 */
class Rule {

private:
	string name; /**< Rule name. */
	Graph * g; /**< Reference to the graph on which the rule is performed. */
	Graph * l; /**< Left-hand side (L) of the rule. */
	Graph * r; /**< Right-hand side (R) of the rule. */
	Graph * res; /**< Resulting graph after the application of the rule. */
	vector<string> common_actors; /**< List of names of common actors between L and R. */  
	vector<string> app_actors; /**< List of names of appearing actors. */
	vector<string> disapp_actors; /**< List of names of disappearing actors. */
	bool connectivity; /**< Boolean to indicate if the rule preserves connectivity (true) or not (false). */
	bool consistency; /**< Boolean to indicate if the rule preserves consistency (true) or not (false). */
	bool liveness; /**< Boolean to indicate if the rule preserves liveness (true) or not (false). */
	bool matching;/**< Boolean to indicate if a matching is found (true) or not (false). */
	bool disappearing_actor;/**< Boolean to indicate if all disappearing actors have explicite edges (true) or not (false). */

	/*!
	 * Check if all disappearing actors have explicite edges.
	 *
	 * \return
	 *
	 */ 
	bool disappearing_actor_check();

	/*!
	 * Check if an edge can match in a graph
	 *
	 * \param g
	 * 	The graph to match
	 *
	 * \param edge
	 * 	The edge to match in the graph
	 *
	 * \return
	 *
	 */ 
	bool match(Graph * g, string edge);

	/*!
	 * Check if a matching of the rule is found in the data graph
	 *
	 * \return
	 *
	 */ 
	bool matching_check();

	/*!
	 * Check if the rule preserves connectivity
	 *
	 * \return
	 *
	 */ 
	bool connectivity_check();

	/*!
	 * Check if the result is connected
	 *
	 * \return
	 *	true if connected, false otherwise.
	 */ 
	bool result_connectivity();

	/*!
	 * Check if the rule preserves consistency
	 *
	 * \return
	 *
	 */ 
	bool consistency_check();

	/*!
	 * Check if the result is consistent
	 *
	 * \return
	 *	true if consistent, false otherwise.
	 */ 
	bool result_consistency();

	/*!
	 * Check if the rule preserves liveness
	 *
	 * \return
	 *
	 */ 
	bool liveness_check();

	/*!
	 * Check if the result is live
	 *
	 * \return
	 *	true if live, false otherwise.
	 */ 
	bool result_liveness();

	/*!
	 * Extract all common, appearing, and disappearing actors
	 *
	 */ 
	void extract_common_actors();

	/*!
	 * Apply the transformation rule to the graph and produce the resulting graph
	 *
	 * \return
	 *
	 */ 
	int apply();
public:
	
	/*!
	 * Constructor of the rule class
	 *
	 * \param name
	 * 	The name of the rule.
	 *
	 */ 
	Rule(string name);
	
	/*!
	 * Get the name of the rule
	 *
	 * \return
	 * 	The name of the rule.
	 *
	 */ 
	string get_name();
	
	/*!
	 * Return the left-hand side of the transformation
	 *
	 * \return
	 *
	 */ 
	Graph * left();
	
	/*!
	 * Return the right-hand side of the transformation
	 *
	 * \return
	 *
	 */ 
	Graph * right();
	
	/*!
	 * Return the result of the transformation
	 *
	 * \return
	 *
	 */ 
	Graph * result();
	
	/*!
	 * Verify if the transformation is valid and write the message to the console.
	 *
	 * \return
	 *
	 */ 
	int verify();

	/*!
	 * Verify and apply the transformation and produce the result if possible.
	 *
	 * \param g
	 * 	The RDF graph on which the transformation will be applied.
	 *
	 */ 
	int process(Graph * g);

	/*!
	 * Return benchmark for the rule
	 * The benckmark consists of connectivity, consistency, liveness
	 * processing time for two methods: (1) using verification algorithm
	 * (2) computing the resulting graph and perform analysis on it.
	 *
	 * \return
	 * 	A string formatted "rule name, connectivity process time, 
	 * 	consistency processing time, liveness processing time,
	 * 	result connectivity process time, result consistency process time,
	 * 	result liveness process time".
	 *
	 */
	string benchmark();

};

#endif
