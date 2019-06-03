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

#include "int_vector_producer.h"

using namespace df;
using namespace std;

ActorRegister<IntVectorProducer> IntVectorProducer::reg("IntVectorProducer");

IntVectorProducer::IntVectorProducer(const string& name) : Actor(name) {
  output = createOutputPort<IntVector>("output");
}

void IntVectorProducer::init() {
 
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

void IntVectorProducer::run() {

  IntVector * out = produce(output);	

  srand(time(NULL));

  msg = "producing ";

  for (int k=0; k<16; k++) {
	auto n = rand()%100;
	msg += to_string(n)+" ";
  	out->put_item(n);
  }

  log(msg);
  timer.randSleep(200);

  if(number >= last)
    setEos(output);

  number += step;
  release(output);  

}

IntVectorProducer::~IntVectorProducer() {
  destroyPort(output);
}
