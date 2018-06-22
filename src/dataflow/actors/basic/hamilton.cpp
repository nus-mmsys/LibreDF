/*
 *   libdataflow
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

#include "hamilton.h"

using namespace df;
using namespace std;

ActorRegister<Hamilton> Hamilton::reg("Hamilton");

Hamilton::Hamilton(const string & name) : Actor(name) {
  input = createInputPort<string>("input");
  output = createOutputPort<string>("output");
}

void Hamilton::init() {
  if (!propEmpty("nbnodes")) {
    nbnodes = getPropInt("nbnodes");
  } else
    nbnodes = 0;
  
  string * out = produce(output);
  *out = name;
  release(output);

}
void Hamilton::run() {

  // Does not yet work.
  // 1. Edges are not two ways.
  // 2. Port numbers can be variable.
	
  string msg;
  string * in = consume(input);
  if ((*in).find(name)==string::npos) {
    msg = *in + name;
  } else
    msg = "";
  release(input);

  if (msg.length() == nbnodes) {
    log("hamiltonian path "+msg+"\n");
  } else {
    string * out = produce(output);
    log("sending "+*out);
    release(output);
  }
   
  sleep(100);
}

Hamilton::~Hamilton() {
  destroyPort(input);
  destroyPort(output);
}
