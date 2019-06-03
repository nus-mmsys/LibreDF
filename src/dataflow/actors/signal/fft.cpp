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

#include "fft.h"

using namespace df;
using namespace std;

ActorRegister<FFT> FFT::reg("FFT");

FFT::FFT(const string& name) : Actor(name) {
  input = createInputPort<ComplexVector>("input");
  output1 = createOutputPort<ComplexVector>("output1");
  output2 = createOutputPort<ComplexVector>("output2");
}

void FFT::init() {
 
}

void FFT::run() {

  ComplexVector * in = consume(input);
  ComplexVector * out1 = produce(output1);
  ComplexVector * out2 = produce(output2);

  out1->clear_items();
  out2->clear_items();

  const int m = in->vector_size()/2;
  const double theta0 = 2*M_PI/in->vector_size();

  for (int p = 0; p < m; p++) {
            const std::complex<double> wp = cos(p*theta0)+i*(-sin(p*theta0));
            const std::complex<double> a = in->get_item(p + 0);
            const std::complex<double> b = in->get_item(p + m);
            out2->put_item(a + b);
            out1->put_item((a - b) * wp);
        }

  release(output1);  
  release(output2);  
  release(input);
}


FFT::~FFT() {
  destroyPort(input);
  destroyPort(output1);
  destroyPort(output2);
}
