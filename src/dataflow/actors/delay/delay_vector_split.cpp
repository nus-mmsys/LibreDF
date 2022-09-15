/*
 *   LibreDF
 *   Copyright (C) A. Shafiei
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

#include "delay_vector_split.h"

using namespace df;
using namespace std;

ActorRegister<DelayVectorSplit> DelayVectorSplit::reg("DelayVectorSplit");

DelayVectorSplit::DelayVectorSplit(const string& name) : Actor(name) {
  input = createInputPort<df::IntVector>("input");
  output = createOutputPortVector<df::IntVector>("output");
}

void DelayVectorSplit::init() {
  if (!propEmpty("delay"))
    delay = getPropInt("delay");
  else
    delay = 1;

  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");

  tilew = 0;
  tileh = 0;
  max = delay*720000;
}

void DelayVectorSplit::reinit() {

  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");
  
  tilew = 0;
  tileh = 0;
  max = delay*720000;
}

void DelayVectorSplit::run() {

  auto in = consumeMR(input);	
  auto out = produce(output);

  start_exec = timer.nowUs();
  
  for (int j=0; j < in.size(); j++) {
	out[j]->clear_items();
	for (int k=0;  k<in[j]->vector_size(); k++) {
  	  	out[j]->put_item(in[j]->get_item(k));
	}
  }

  log("delay split "+to_string(stepno));

  counter = 0;
  while(counter<max) counter++;

  end_exec = timer.nowUs();
  
  releaseMR(input);
  release(output);

}

DelayVectorSplit::~DelayVectorSplit() {
  destroyPort(input);
  destroyPort(output);
}
