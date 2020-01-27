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

#include "delay_join.h"

using namespace df;
using namespace std;

ActorRegister<DelayJoin> DelayJoin::reg("DelayJoin");

DelayJoin::DelayJoin(const string& name) : Actor(name) {
  input = createInputPortVector<df::Int>("input");
  output = createOutputPort<df::Int>("output");
}

void DelayJoin::init() {
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

void DelayJoin::reinit() {

  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");

  tilew = 0;
  tileh = 0;

}

void DelayJoin::run() {

  auto in = consume(input);	
  auto out = produceMR(output);

  start_exec = timer.nowUs();
  
  for (int j=0; j<out.size(); j++) {
	  out[j]->set(*in[j]->get());
  }

  log("delay join "+to_string(stepno));
  
  counter = 0;
  while(counter<max) counter++;

  end_exec = timer.nowUs();
  
  release(input);
  releaseMR(output);

}

DelayJoin::~DelayJoin() {
  destroyPort(input);
  destroyPort(output);
}
