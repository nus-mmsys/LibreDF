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

#include "hamilton.h"

using namespace df;
using namespace std;

ActorRegister<Hamilton> Hamilton::reg("Hamilton");

Hamilton::Hamilton(const string & name) : Actor(name) {
  input = createInputPortVector<Str>("input");
  output = createOutputPortVector<Str>("output");
}

void Hamilton::init() {
  if (!propEmpty("nbnodes")) {
    nbnodes = getPropInt("nbnodes");
  } else
    nbnodes = 0;
  
  auto out = produce(output);
  for (auto o : out)
  	o->set(name);
  release(output);

}
void Hamilton::run() {

  string msg;
  auto in = consume(input);
  auto out = produce(output);
  for (auto i : in) {
  	if (i->get()->find(name)==string::npos) {
    		msg = *i->get() + name;
  	} else
    		msg = "";

  	if (msg.length() == nbnodes) {
    		log("hamiltonian path "+msg+"\n");
  	} else if (msg != "") {
		for (auto o : out) {
			o->set(msg);
    			log("sending "+msg);
		}
	}
  }
  release(input); 
  release(output);
}

Hamilton::~Hamilton() {
  destroyPort(input);
  destroyPort(output);
}
