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

#include "void_source.h"

using namespace df;
using namespace std;

ActorRegister<VoidSource> VoidSource::reg("VoidSource");

VoidSource::VoidSource(const std::string & name): df::Actor(name) {
  output = createOutputPort<df::Int>("output");
}

void VoidSource::init() {
  if (!propEmpty("last"))  	
  	last = getPropInt("last");
  else
        last = 1000;
}

void VoidSource::reinit() {
}

void VoidSource::run() {
  auto out = produce(output);

  start_exec = timer.nowUs();

  out->set(stepno);
  log("void source "+to_string(stepno));

  if(stepno >= last)
    setEos(output);

  end_exec = timer.nowUs();

  release(output);
}

VoidSource::~VoidSource() {
  destroyPort(output);
}
