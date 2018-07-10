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

#ifndef DF_SERVER_H
#define DF_SERVER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <functional>
#include "parser/parser.h"
#include "dataflow/core/df.h"

#define SERVER_PORT 7070

using namespace std;

/*!
 * \class Server
 *
 */
class Server {

private:
	df::ServerSocket * sock;
	Parser * parser;
protected:
	Graph * graph;

public:

	/*!
	 * Server constructor
	 *
	 *  \param parser
	 *
	 */ 
	Server(Parser * parser);
	
	int init();
	int run();
};

#endif
