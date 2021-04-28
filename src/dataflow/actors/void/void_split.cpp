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

#include "void_split.h"

using namespace df;
using namespace std;

ActorRegister<VoidSplit> VoidSplit::reg("VoidSplit");

VoidSplit::VoidSplit(const string& name) : Actor(name) {
  input = createInputPort<df::Int>("input");
  output = createOutputPortVector<df::Int>("output");
}

void VoidSplit::init() {
  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");
}

void VoidSplit::reinit() {

  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");
}

void VoidSplit::run() {

  auto in = consumeMR(input);	
  auto out = produce(output);

  start_exec = timer.nowUs();
  
  for (int j=0; j < in.size(); j++) {
	out[j]->set(*in[j]->get());
  }

  log("void split "+to_string(stepno));

  end_exec = timer.nowUs();
  
  releaseMR(input);
  release(output);

}

VoidSplit::~VoidSplit() {
  destroyPort(input);
  destroyPort(output);
}
