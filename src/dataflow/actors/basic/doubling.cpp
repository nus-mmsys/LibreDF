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

#include "doubling.h"

using namespace df;
using namespace std;

ActorRegister<Doubling> Doubling::reg("Doubling");

Doubling::Doubling(const string & name) :
Actor(name) {
  input = createInputPort<Int>("input");
  output = createOutputPort<Int>("output");
}

void Doubling::init() {

}

void Doubling::run() {

  Int * in = consume(input);	
  int outval = *in->get() * 2;
  release(input);
  
  Int * out = produce(output);
  out->set(outval);
  log("producing "+to_string(outval));
  release(output);
}

Doubling::~Doubling() {
  destroyPort(input);
  destroyPort(output);
}
