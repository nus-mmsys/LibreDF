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

#ifndef DF_ANALYSER_H
#define DF_ANALYSER_H

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "graph.h"

using namespace std;

/*!
 * \class Analyser
 */
class Analyser {

protected:
	Graph * graph;

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
	
public:
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

};

#endif
