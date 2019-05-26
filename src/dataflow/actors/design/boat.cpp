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

#include "boat.h"

using namespace df;
using namespace std;

ActorRegister<Boat> Boat::reg("Boat");

Boat::Boat(const string& name) : Actor(name) {
  input = createInputPort<Int>("input");
  output = createOutputPort<Complex3D>("output");

}

void Boat::init() {
 
  if (!propEmpty("last"))  	
  	last = getPropInt("last");
  else
        last = 100;
  
}

void Boat::run() {

  Int * in = consume(input);
  Complex3D * out = produce(output);
  int k = *in->get();
  out->get()->x = exp((0,pi()/4)) * 
           (sin(6*pi()*k/2000 + pi()/4), 
            sin(12*pi()*k/2000));

  out->get()->y = exp((0,pi()/4)) * 
           (sin(4*pi()*k/2000 + pi()/6),
            sin(2*pi()*k/2000 + pi()/3)) ;    

  log("producing "+to_string(stepno));
  
  release(output);  
  release(input);
}


Boat::~Boat() {
  destroyPort(output);
}
