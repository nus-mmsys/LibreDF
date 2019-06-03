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

#include "sort_split.h"

using namespace df;
using namespace std;

ActorRegister<SortSplit> SortSplit::reg("SortSplit");

SortSplit::SortSplit(const string& name) : Actor(name) {
  input = createInputPort<IntVector>("input");
  output1 = createOutputPort<IntVector>("output1");
  output2 = createOutputPort<IntVector>("output2");
}

void SortSplit::init() {
 
}

void SortSplit::run() {

  IntVector * in = consume(input);
  IntVector * out1 = produce(output1);
  IntVector * out2 = produce(output2);

  out1->clear_items();
  out2->clear_items();

  const int m = in->vector_size()/2;

  for (int p = 0; p < m; p++) {
            out1->put_item(in->get_item(p+0));
            out2->put_item(in->get_item(p+m));
  }

  release(output1);  
  release(output2);  
  release(input);
}


SortSplit::~SortSplit() {
  destroyPort(input);
  destroyPort(output1);
  destroyPort(output2);
}
