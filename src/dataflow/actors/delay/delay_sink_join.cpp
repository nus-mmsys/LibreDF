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

#include "delay_sink_join.h"

using namespace df;
using namespace std;

ActorRegister<DelaySinkJoin> DelaySinkJoin::reg("DelaySinkJoin");

DelaySinkJoin::DelaySinkJoin(const std::string & name): df::Actor(name) {
  input = createInputPortVector<df::Int>("input");
}

void DelaySinkJoin::init() {
  if (!propEmpty("delay"))
    delay = getPropInt("delay");
  else
    delay = 1;

  if (propEmpty("arity"))
    arity = 1;
  else
    arity = getPropInt("arity");

  max = delay * 720000;
}

void DelaySinkJoin::reinit() {
  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");

  max = delay*720000;
}

void DelaySinkJoin::run() {

  auto in = consume(input);
  
  start_exec = timer.nowUs();
 
  for (int j=0; j<arity; j++) {
	  val = *in[j]->get();
  }
 
  log("delay sink join "+to_string(stepno));
 
  counter = 0;
  while(counter<max) counter++;

  end_exec = timer.nowUs();
  
  release(input);
}

DelaySinkJoin::~DelaySinkJoin() {
  destroyPort(input);
}

