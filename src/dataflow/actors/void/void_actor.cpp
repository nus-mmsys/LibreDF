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

#include "void_actor.h"

using namespace df;
using namespace std;

ActorRegister<VoidActor> VoidActor::reg("VoidActor");

VoidActor::VoidActor(const std::string & name): df::Actor(name) {
  input = createInputPort<df::Int>("input");
  output = createOutputPort<df::Int>("output");
}

void VoidActor::init() {
}

void VoidActor::reinit() {
}

void VoidActor::run() {
  auto in = consume(input);
  auto out = produce(output);

  start_exec = timer.nowUs();

  out->set(*in->get());
  log("void actor "+to_string(stepno));

  end_exec = timer.nowUs();
  
  release(input);
  release(output);
}

VoidActor::~VoidActor() {
  destroyPort(input);
  destroyPort(output);
}

