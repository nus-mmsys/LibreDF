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

int Hamilton2::path_length(const string & msg) {
	return std::count(msg.begin(), msg.end(), ',')+1; 
}

bool Hamilton2::hamiltonian(const string & msg) {
	string node, partmsg=msg;
  	std::stringstream ss(msg);
  	while (partmsg!="") {
		node=partmsg.substr(0,partmsg.find(","));
		partmsg=partmsg.substr(partmsg.find(",")+1);
		if (partmsg.find(node) != string::npos)
			return false;
	}
	return true;
}
void Hamilton2::init() {
  if (!propEmpty("nbnodes")) {
    nbnodes = getPropInt("nbnodes");
  } else
    nbnodes = 0;
  loop_list = new vector<string>[nbnodes];
}

void Hamilton2::start() {
	
	auto out = produce(output);
  	for (auto o : out) {
  		o->set(name+";");
  	}
  	release(output);
}

void Hamilton2::run() {

  input_messages = "";
  auto in = consume(input);
  for (auto i : in) {
	input_messages = *i->get() + input_messages;
  }
  release(input);

  std::stringstream ss(input_messages);

  output_message = "";
  while (std::getline(ss, imsg, ';')) {
  	  
	if (imsg!="" && imsg.find(name)==string::npos) {

		for (auto l : loop_list[nbnodes-path_length(imsg)]) {
			if (hamiltonian(imsg+","+l)) {
    				log("Hamiltonian path: "+imsg+","+l);
				continue;
			}
		}

		omsg = imsg + "," + name;

	  	if (path_length(omsg) == nbnodes) {
    			log("Hamiltonian path: "+omsg);
		} else {
			output_message = omsg + ";" + output_message;
		}		
  	} else if (imsg!="") {
		loop = imsg.substr(imsg.find(name));
		loop_list[path_length(loop)].push_back(loop);	
	}
  }

  auto out = produce(output);
  for (auto o : out) {
	o->set(output_message);
  }

  if (stepno >= nbnodes-1) {
	  setEos(output);
  }

  release(output);
}

Hamilton2::~Hamilton2() {
  destroyPort(input);
  destroyPort(output);
}
