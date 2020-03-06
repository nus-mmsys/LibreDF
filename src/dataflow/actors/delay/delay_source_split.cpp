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

#include "delay_source_split.h"

using namespace df;
using namespace std;

ActorRegister<DelaySourceSplit> DelaySourceSplit::reg("DelaySourceSplit");

DelaySourceSplit::DelaySourceSplit(const std::string & name): df::Actor(name) {
  output = createOutputPortVector<df::Int>("output");
}

void DelaySourceSplit::init() {
  if (!propEmpty("delay"))
    delay = getPropInt("delay");
  else
    delay = 1;

  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");

  if (!propEmpty("last"))  	
  	last = getPropInt("last");
  else
        last = 1000;

  max = delay*720000;
}

void DelaySourceSplit::reinit() {
  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");
  
  max = delay*720000;
}

void DelaySourceSplit::run() {

  auto out = produce(output);

  start_exec = timer.nowUs();

  for (int j=0; j < arity; j++) {
	out[j]->set(stepno);
  }

  log("delay source split "+to_string(stepno));

  counter = 0;
  while(counter<max) counter++;

  if(stepno >= last)
    setEos(output);

  end_exec = timer.nowUs();

  release(output);
}

DelaySourceSplit::~DelaySourceSplit() {
  destroyPort(output);
}
