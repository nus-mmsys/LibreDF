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

#include "void_join.h"

using namespace df;
using namespace std;

ActorRegister<VoidJoin> VoidJoin::reg("VoidJoin");

VoidJoin::VoidJoin(const string& name) : Actor(name) {
  input = createInputPortVector<df::Int>("input");
  output = createOutputPort<df::Int>("output");
}

void VoidJoin::init() {
  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");
}

void VoidJoin::reinit() {
  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");
}

void VoidJoin::run() {

  auto in = consume(input);	
  auto out = produceMR(output);

  start_exec = timer.nowUs();
  
  for (int j=0; j<out.size(); j++) {
	  out[j]->set(*in[j]->get());
  }

  log("void join "+to_string(stepno));

  end_exec = timer.nowUs();
  
  release(input);
  releaseMR(output);

}

VoidJoin::~VoidJoin() {
  destroyPort(input);
  destroyPort(output);
}
