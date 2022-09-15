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

#include "addition.h"

using namespace df;
using namespace std;

ActorRegister<Addition> Addition::reg("Addition");

Addition::Addition(const string & name) :
Actor(name) {
  input1 = createInputPort<Int>("input1");
  input2 = createInputPort<Int>("input2");
}

void Addition::init() {

}

void Addition::run() {
  
  Int * in1 = consume(input1);
  
  Int * in2 = consume(input2);
  
  int out = *in1->get() + *in2->get();
  
  log("addition "+to_string(out));
  
  release(input1);
  release(input2);
}

Addition::~Addition() {
  destroyPort(input1);
  destroyPort(input2);
}

