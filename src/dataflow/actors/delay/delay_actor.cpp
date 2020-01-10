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

#include "delay_actor.h"

using namespace df;
using namespace std;

ActorRegister<DelayActor> DelayActor::reg("DelayActor");

DelayActor::DelayActor(const std::string & name): df::Actor(name) {
  input = createInputPort<df::Mat>("input");
  output = createOutputPort<df::Mat>("output");
}

void DelayActor::init() {
  if (!propEmpty("delay"))
    delay = getPropInt("delay");
  else
    delay = 1;
}

void DelayActor::reinit() {
}

void DelayActor::run() {
  auto in = consume(input);
  auto out = produce(output);
  out->set(*in->get());
  log("delay actor "+to_string(stepno));

  timer.sleep(delay);

  release(input);
  release(output);
}

DelayActor::~DelayActor() {
  destroyPort(input);
  destroyPort(output);
}

