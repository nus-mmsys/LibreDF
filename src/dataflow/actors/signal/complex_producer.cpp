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

#include "complex_producer.h"

using namespace df;
using namespace std;

const double pi = std::acos(-1);
const std::complex<double> i(0,1);

ActorRegister<ComplexProducer> ComplexProducer::reg("ComplexProducer");

ComplexProducer::ComplexProducer(const string& name) : Actor(name) {
  output = createOutputPort<ComplexVector>("output");
}

void ComplexProducer::init() {
 
  number = 0; 
  if (!propEmpty("step"))  	
  	step = getPropInt("step");
  else
        step = 1;

  if (!propEmpty("last"))  	
  	last = getPropInt("last");
  else
        last = 100;
  
}

void ComplexProducer::run() {

  ComplexVector * out = produce(output);	
  
  out->clear_items();

  msg = "producing ";
  for (int k=0; k<16; k++) {
	//std::complex<double> c = exp(i*2*k*M_PI/16.0);
	std::complex<double> c = cos(2*k*M_PI/16)+i*sin(2*k*M_PI/16);
  	out->put_item(c);
	msg += to_string(c.real()) + "," + to_string(c.imag()) + "i  "; 
  }

  log(msg);

  timer.randSleep(200);

  if(number >= last)
    setEos(output);

  number += step;
  release(output);  

}

ComplexProducer::~ComplexProducer() {
  destroyPort(output);
}
