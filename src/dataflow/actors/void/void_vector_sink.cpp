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

#include "void_vector_sink.h"

using namespace df;
using namespace std;

ActorRegister<VoidVectorSink> VoidVectorSink::reg("VoidVectorSink");

VoidVectorSink::VoidVectorSink(const std::string & name): df::Actor(name) {
  input = createInputPort<df::IntVector>("input");
}

void VoidVectorSink::init() {
}

void VoidVectorSink::reinit() {
}

void VoidVectorSink::run() {
  auto in = consume(input);
  
  start_exec = timer.nowUs();

  for (int k=0; k<in->vector_size(); k++) {
	  val = in->get_item(k);
  	  log("void sink reads "+to_string(val));
  }  

  log("void sink "+to_string(stepno));
 
  end_exec = timer.nowUs();
  
  release(input);
}

VoidVectorSink::~VoidVectorSink() {
  destroyPort(input);
}

