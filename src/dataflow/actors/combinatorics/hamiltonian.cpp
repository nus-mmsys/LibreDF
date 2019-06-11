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

#include "hamiltonian.h"

using namespace df;
using namespace std;

ActorRegister<Hamiltonian> Hamiltonian::reg("Hamiltonian");

Hamiltonian::Hamiltonian(const string & name) : Actor(name) {
  input = createInputPortVector<Msg>("input");
  output = createOutputPortVector<Msg>("output");
}

void Hamiltonian::init() {
  if (!propEmpty("nbnodes")) {
    nbnodes = getPropInt("nbnodes");
  } else
    nbnodes = 0;
  first = true;  
}
void Hamiltonian::run() {

  if (first) {
	auto out = produce(output);
  	for (auto o : out) {
  		o->setHeader(1);
		o->setMessage(name+";");
  	}
	release(output);
	first = false;
	log("First execution terminated.");
	return;
  }
/*
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
    		
		omsg = imsg + "," + name;

	  	if (std::count(omsg.begin(), omsg.end(), ',') == nbnodes-1) {
    			log("Hamiltonianian path: "+omsg);
		} else {
			output_message = omsg + ";" + output_message;
		}		
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
  */
}

Hamiltonian::~Hamiltonian() {
  destroyPort(input);
  destroyPort(output);
}