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
  input = createInputPortVector<MsgVector>("input");
  output = createOutputPortVector<MsgVector>("output");
}

void Hamiltonian::init() {
  if (!propEmpty("nbnodes")) {
    nbnodes = getPropInt("nbnodes");
  } else
    nbnodes = 0;
  first = true;  
  int iname=0;
  
  try {
	iname = stoi(name);
	prime = arith.nThPrime(iname);
  } catch(...) {
	prime = 2;
	log("Hamiltonian init: name cannot be converted to integer.");
  }
}
void Hamiltonian::run() {

  if (first) {
	auto out = produce(output);
  	for (auto o : out) {
  		o->put_item(prime, name);
  	}
	release(output);
	first = false;
	return;
  }

  auto in = consume(input);
  auto out = produce(output);

  for (auto items : in) {
	  for (int i=0; i<items->vector_size();i++) {
  	 	
		auto h = items->get_item_header(i);
		auto m = items->get_item_message(i);

		if (h % prime != 0) {
    		
			omsg = m + "," + name;

	  		if (std::count(omsg.begin(), omsg.end(), ',') == nbnodes-1) {
    				log("Hamiltonianian path: "+omsg);
			} else {
				for (auto o : out) {
					o->put_item(h*prime,omsg);
  				}
			}		
  		}
	  }
  }
  
  release(input);
  
  if (stepno >= nbnodes) {
	setEos(output);		
  }

  release(output);
 
}

Hamiltonian::~Hamiltonian() {
  destroyPort(input);
  destroyPort(output);
}
