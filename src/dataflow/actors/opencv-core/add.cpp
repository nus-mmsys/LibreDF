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

#include "add.h"

using namespace df;
using namespace std;

ActorRegister<Add> Add::reg("Add");

Add::Add(const string& name) : Actor(name) {
  input1 = createInputPort<df::Mat>("input1");
  input2 = createInputPort<df::Mat>("input2");
  output = createOutputPort<df::Mat>("output");
}

void Add::init() {


}

void Add::run() {

  auto in1 = consume(input1);	
  auto in2 = consume(input2);	
  auto out = produce(output);
  out->set(*in1->get() + *in2->get());
  log("sending "+to_string(stepno));
  release(input1);
  release(input2);
  release(output);

}

Add::~Add() {
  destroyPort(input1);
  destroyPort(input2);
  destroyPort(output);
}
