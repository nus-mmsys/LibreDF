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

#include "sort2.h"

using namespace df;
using namespace std;

ActorRegister<Sort2> Sort2::reg("Sort2");

Sort2::Sort2(const string& name) : Actor(name) {
  input = createInputPort<IntVector>("input");
  output = createOutputPort<IntVector>("output");
}

void Sort2::init() {
 
}

void Sort2::run() {

  IntVector * in = consume(input);
  IntVector * out = produce(output);

  out->clear_items();

  int a = in->get_item(0);
  int b = in->get_item(1);

  if (a<b) {
	  out->put_item(a);
	  out->put_item(b);
  } else {
	  out->put_item(b);
	  out->put_item(a);
  }

  release(output);  
  release(input);
}

Sort2::~Sort2() {
  destroyPort(input);
  destroyPort(output);
}
