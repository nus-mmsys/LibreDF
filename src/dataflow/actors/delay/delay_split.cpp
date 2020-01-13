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

#include "delay_split.h"

using namespace df;
using namespace std;

ActorRegister<DelaySplit> DelaySplit::reg("DelaySplit");

DelaySplit::DelaySplit(const string& name) : Actor(name) {
  input = createInputPort<df::Int>("input");
  output = createOutputPortVector<df::Int>("output");
}

void DelaySplit::init() {
  if (!propEmpty("delay"))
    delay = getPropInt("delay");
  else
    delay = 1;

  max = delay * 695000;
  
  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");

  tilew = 0;
  tileh = 0;
}

void DelaySplit::reinit() {

  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");
  
  tilew = 0;
  tileh = 0;
}

void DelaySplit::run() {

  auto in = consumeMR(input);	
  auto out = produce(output);

  for (int j=0; j < in.size(); j++) {
	out[j]->set(*in[j]->get());
  }

  log("delay split "+to_string(stepno));

  counter = 0;
  while(counter<max) counter++;

  releaseMR(input);
  release(output);

}

DelaySplit::~DelaySplit() {
  destroyPort(input);
  destroyPort(output);
}
