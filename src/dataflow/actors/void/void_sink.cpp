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

#include "void_sink.h"

using namespace df;
using namespace std;

ActorRegister<VoidSink> VoidSink::reg("VoidSink");

VoidSink::VoidSink(const std::string & name): df::Actor(name) {
  input = createInputPort<df::Int>("input");
}

void VoidSink::init() {
}

void VoidSink::reinit() {
}

void VoidSink::run() {
  auto in = consume(input);
  
  start_exec = timer.nowUs();
  
  log("void sink "+to_string(stepno));
 
  end_exec = timer.nowUs();
  
  release(input);
}

VoidSink::~VoidSink() {
  destroyPort(input);
}

