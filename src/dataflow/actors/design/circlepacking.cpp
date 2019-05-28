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

#include "circlepacking.h"

using namespace df;
using namespace std;

ActorRegister<CirclePacking> CirclePacking::reg("CirclePacking");

CirclePacking::CirclePacking(const string& name) : Actor(name) {
  input = createInputPort<Int>("input");
  output = createOutputPort<Float3D>("output");
}

void CirclePacking::init() {
 
}

void CirclePacking::run() {

  Int * in = consume(input);
  Float3D * out = produce(output);
  int k = *in->get();
  out->get()->x = cos(k*pi);

  out->get()->y = sin(k*pi); 

  out->get()->z = 1;

  log("producing "+to_string(stepno));
  
  release(output);  
  release(input);
}


CirclePacking::~CirclePacking() {
  destroyPort(output);
}
