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

#include "delay_generic.h"

using namespace df;
using namespace std;

ActorRegister<DelayGeneric> DelayGeneric::reg("DelayGeneric");

DelayGeneric::DelayGeneric(const std::string & name): df::Actor(name) {
  input = createInputPortVector<df::Int>("input");
  output = createOutputPortVector<df::Int>("output");
}

void DelayGeneric::init() {
  if (!propEmpty("delay"))
    delay = getPropInt("delay");
  else
    delay = 1;
  max = delay * 720000;
}

void DelayGeneric::reinit() {
}

void DelayGeneric::run() {
  auto in = consume(input);
  auto out = produce(output);

  start_exec = timer.nowUs();
  
  for (i=0; i<in.size(); i++) sum+= *in[i]->get();
  for (i=0; i<out.size(); i++)
  	out[i]->set(sum);

  log("delay generic "+to_string(stepno));

  counter = 0;
  while(counter < max) counter++;

  end_exec = timer.nowUs();
  
  release(input);
  release(output);
}

DelayGeneric::~DelayGeneric() {
  destroyPort(input);
  destroyPort(output);
}

