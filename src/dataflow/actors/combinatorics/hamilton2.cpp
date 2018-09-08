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

#include "hamilton2.h"

using namespace df;
using namespace std;

ActorRegister<Hamilton2> Hamilton2::reg("Hamilton2");

Hamilton2::Hamilton2(const string & name) : Actor(name) {
  input = createInputPortVector<Str>("input");
  output = createOutputPortVector<Str>("output");
}

int path_length(const string & msg) {
	return std::count(msg.begin(), msg.end(), ','); 
}

bool hamiltonian(const string & msg) {
	//TODO
	return false;
}
void Hamilton2::init() {
  if (!propEmpty("nbnodes")) {
    nbnodes = getPropInt("nbnodes");
  } else
    nbnodes = 0;
  first = true;  
  loop_list = new vector<string>[nbnodes];
}
void Hamilton2::run() {

  if (first) {
	auto out = produce(output);
  	for (auto o : out) {
  		o->set(name);
  	}
  	release(output);
	first = false;
	return;
  }

  input_messages = "";
  auto in = consume(input);
  for (auto i : in) {
	input_messages = *i->get() + ";" + input_messages;
  }
  release(input);

  std::stringstream ss(input_messages);

  output_message = "";
  while (std::getline(ss, msg, ';')) {
  	  
	if (msg!="" && msg.find(name)==string::npos) {

		for (auto l : loop_list[nbnodes-path_length(msg)]) {
			if (hamiltonian(msg+","+l)) {
    				log("Hamiltonian path: "+msg+","+l);
				continue;
			}
		}

		msg = msg + "," + name;

	  	if (path_length(msg) == nbnodes-1) {
    			log("Hamiltonian path: "+msg);
		} else {
			output_message = msg + ";" + output_message;
		}		
  	} else if (msg!="") {
		loop = msg.substr(msg.find(name));
		loop_list[path_length(loop)].push_back(loop);	
	}
  }

  auto out = produce(output);
  for (auto o : out) {
	o->set(output_message);
  }
  release(output);
}

Hamilton2::~Hamilton2() {
  destroyPort(input);
  destroyPort(output);
}