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

#include "delay_vector_actor2o.h"

using namespace df;
using namespace std;

ActorRegister<DelayVectorActor2O> DelayVectorActor2O::reg("DelayVectorActor2O");

DelayVectorActor2O::DelayVectorActor2O(const std::string & name): df::Actor(name) {
  input = createInputPort<df::IntVector>("input");
  output1 = createOutputPort<df::IntVector>("output1");
  output2 = createOutputPort<df::IntVector>("output2");
}

void DelayVectorActor2O::init() {
  if (!propEmpty("delay"))
    delay = getPropInt("delay");
  else
    delay = 1;
  max = delay * 720000;
}

void DelayVectorActor2O::reinit() {
}

void DelayVectorActor2O::run() {
  auto in = consume(input);
  auto out1 = produce(output1);
  auto out2 = produce(output2);

  start_exec = timer.nowUs();

  out1->clear_items();
  out2->clear_items();
  for (int k=0; k<in->vector_size(); k++) {
	  out1->put_item(in->get_item(k));
	  out2->put_item(in->get_item(k));
  }

  log("delay actor "+to_string(stepno));

  counter = 0;
  while(counter < max) counter++;

  end_exec = timer.nowUs();
  
  release(input);
  release(output1);
  release(output2);
}

DelayVectorActor2O::~DelayVectorActor2O() {
  destroyPort(input);
  destroyPort(output1);
  destroyPort(output2);
}

