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

#include "deploy.h"

Deploy::Deploy(int argc, char * argv[], Parser * p) {

	if (argc != 2) {
		cout << "usage: " << argv[0] << " <file.df>\n";
		exit(0);
	}
	parser = p;		
	parser->load_from_file(argv[1]);
	sock = new df::ClientSocket("tmf-deploy");
	serverip = "127.0.0.1";
}

int Deploy::run() {

	sock->connect(serverip, SERVER_PORT);
	std::string msg = parser->get_df_string();
	sock->send(msg.c_str(), msg.size());
	sock->close();

	return 0;
}
