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

#include "flower.h"

using namespace df;
using namespace std;

ActorRegister<Flower> Flower::reg("Flower");

Flower::Flower(const string& name) : Actor(name) {
  input = createInputPort<Int>("input");
  output = createOutputPort<Float3D>("output");
}

void Flower::init() {
 
}

void Flower::run() {

  Int * in = consume(input);
  Float3D * out = produce(output);
  int k = *in->get();
  out->get()->x = sin(2 * pi * k/7000.0) * 
           (1 - 1/3.0*pow(cos(8*pi*k/7000.0),4) - 
	   1/4.0*pow(cos(36*pi*k/7000.0),4) -
	   1/4.0*pow(cos(12*pi*k/7000.0),4));

  out->get()->y = cos(2 * pi * k/7000.0) * 
           (1 - 1/3.0*pow(cos(8*pi*k/7000.0),4) - 
	   1/4.0*pow(cos(36*pi*k/7000.0),4) -
	   1/4.0*pow(cos(12*pi*k/7000.0),4));

  out->get()->z = 1/200.0 + 1/9.0*(pow(cos(36*pi*k/7000.0),2));

  log("producing "+to_string(stepno));
  
  release(output);  
  release(input);
}


Flower::~Flower() {
  destroyPort(output);
}
