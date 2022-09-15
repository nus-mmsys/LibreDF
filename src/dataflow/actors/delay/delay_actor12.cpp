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

#include "delay_actor12.h"

using namespace df;
using namespace std;

ActorRegister<DelayActor12> DelayActor12::reg("DelayActor12");

DelayActor12::DelayActor12(const std::string & name): df::Actor(name) {
  input = createInputPort<df::Int>("input");
  output1 = createOutputPort<df::Int>("output1");
  output2 = createOutputPort<df::Int>("output2");
}

void DelayActor12::init() {
  if (!propEmpty("delay"))
    delay = getPropInt("delay");
  else
    delay = 1;
  max = delay * 720000;
}

void DelayActor12::reinit() {
}

void DelayActor12::run() {
  auto in = consume(input);
  auto out1 = produce(output1);
  auto out2 = produce(output2);

  start_exec = timer.nowUs();

  out1->set(*in->get());
  out2->set(*in->get());
  log("delay actor "+to_string(stepno));

  counter = 0;
  while(counter < max) counter++;

  end_exec = timer.nowUs();
  
  release(input);
  release(output1);
  release(output2);
}

DelayActor12::~DelayActor12() {
  destroyPort(input);
  destroyPort(output1);
  destroyPort(output2);
}

