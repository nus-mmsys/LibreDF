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

#include "fftjoin.h"

using namespace df;
using namespace std;

ActorRegister<FFTJoin> FFTJoin::reg("FFTJoin");

FFTJoin::FFTJoin(const string& name) : Actor(name) {
  input1 = createInputPort<ComplexVector>("input1");
  input2 = createInputPort<ComplexVector>("input2");
  output = createOutputPort<ComplexVector>("output");
}

void FFTJoin::init() {
 
}

void FFTJoin::run() {

  ComplexVector * in1 = consume(input1);
  ComplexVector * in2 = consume(input2);
  ComplexVector * out = produce(output);

  out->clear_items();

  for (int p = 0; p < in1->vector_size(); p++) {
            out->put_item(in1->get_item(p));
  }
 
  for (int p = 0; p < in2->vector_size(); p++) {
            out->put_item(in2->get_item(p));
  }

  release(input1);  
  release(input2);  
  release(output);
}


FFTJoin::~FFTJoin() {
  destroyPort(input1);
  destroyPort(input2);
  destroyPort(output);
}
