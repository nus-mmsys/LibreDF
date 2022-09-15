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

#include "sort_join.h"

using namespace df;
using namespace std;

ActorRegister<SortJoin> SortJoin::reg("SortJoin");

SortJoin::SortJoin(const string& name) : Actor(name) {
  input1 = createInputPort<IntVector>("input1");
  input2 = createInputPort<IntVector>("input2");
  output = createOutputPort<IntVector>("output");
}

void SortJoin::init() {
 
}

void SortJoin::run() {

  IntVector * in1 = consume(input1);
  IntVector * in2 = consume(input2);
  IntVector * out = produce(output);

  out->clear_items();

  int ii=0,jj=0;
  while (ii < in1->vector_size() && jj < in2->vector_size()) {
          if (in1->get_item(ii) < in2->get_item(jj)) {
	  	out->put_item(in1->get_item(ii));
		ii++;
	  } else {
	  	out->put_item(in2->get_item(jj));
		jj++;
	  }
  }

  while(ii<in1->vector_size()) {
	  out->put_item(in1->get_item(ii));
  	  ii++;
  }

  while(jj<in2->vector_size()) {
	  out->put_item(in2->get_item(jj));
  	  jj++;
  }

  release(input1);  
  release(input2);  
  release(output);
}


SortJoin::~SortJoin() {
  destroyPort(input1);
  destroyPort(input2);
  destroyPort(output);
}
