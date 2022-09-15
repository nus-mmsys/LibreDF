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

#include "server.h"

Server::Server(Parser * p) {
	sock = new df::ServerSocket("df-server");	
	parser = p;
}

int Server::init() {

	int bufsize = 100000;
	char buf[bufsize];

	sock->listen(SERVER_PORT);
	std::cout << "waiting df-deploy...\n";
	sock->accept();
	sock->recv(buf, bufsize);
	std::cout << buf << std::endl;
	parser->load_from_string(buf);
	dfg = parser->get_dataflow();

	sock->clnclose();
	sock->srvclose();

	return 0;
}

int Server::run() {

	// Initialize dataflow
	dfg->init();

	// Connect actors
	dfg->connect();

	// Run dataflow
  	dfg->run();

	return 0;
}
