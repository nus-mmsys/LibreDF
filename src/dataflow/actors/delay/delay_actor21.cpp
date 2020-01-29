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

#include "delay_actor21.h"

using namespace df;
using namespace std;

ActorRegister<DelayActor21> DelayActor21::reg("DelayActor21");

DelayActor21::DelayActor21(const std::string & name): df::Actor(name) {
  input1 = createInputPort<df::Int>("input1");
  input2 = createInputPort<df::Int>("input2");
  output = createOutputPort<df::Int>("output");
}

void DelayActor21::init() {
  if (!propEmpty("delay"))
    delay = getPropInt("delay");
  else
    delay = 1;
  max = delay * 720000;
}

void DelayActor21::reinit() {
}

void DelayActor21::run() {
  auto in1 = consume(input1);
  auto in2 = consume(input2);
  auto out = produce(output);

  start_exec = timer.nowUs();

  out->set(*in1->get() + *in2->get());
  log("delay actor "+to_string(stepno));

  counter = 0;
  while(counter < max) counter++;

  end_exec = timer.nowUs();
  
  release(input1);
  release(input2);
  release(output);
}

DelayActor21::~DelayActor21() {
  destroyPort(input1);
  destroyPort(input2);
  destroyPort(output);
}

