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

#include "delay_sink.h"

using namespace df;
using namespace std;

ActorRegister<DelaySink> DelaySink::reg("DelaySink");

DelaySink::DelaySink(const std::string & name): df::Actor(name) {
  input = createInputPort<df::Int>("input");
}

void DelaySink::init() {
  if (!propEmpty("delay"))
    delay = getPropInt("delay");
  else
    delay = 1;
  
  max = delay * 695000;
}

void DelaySink::reinit() {
}

void DelaySink::run() {
  auto in = consume(input);
  log("delay sink "+to_string(stepno));

  counter = 0;
  while(counter<max) counter++;

  release(input);
}

DelaySink::~DelaySink() {
  destroyPort(input);
}

