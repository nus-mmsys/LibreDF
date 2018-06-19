/*
 *   DF
 *   Copyright (C) 2018
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

#include "int_producer.h"

using namespace df;
using namespace std;

ActorRegister<IntProducer> IntProducer::reg("IntProducer");

IntProducer::IntProducer(const string& name) : Actor(name) {
  outputInt = createOutputPort<int>("output_int");
}

void IntProducer::init() {
 
  if (!propEmpty("last"))  	
  	last = getPropInt("last");
  else
        last = 100;
  
}

void IntProducer::run() {

  if(stepno == last) {
    setEOS(outputInt);
    return;
  }

  int * out = produce(outputInt);	
  *out = stepno;
  log("producing "+to_string(stepno));
  sleep(500);
  release(outputInt);  

}

void IntProducer::runRT() {
  
  string data = to_string(stepno);
  
  bool canlock = outputInt->lockRT();
  
  if (!canlock) {
    log("droping "+data);
    sleep(500);
    return;
  }
  
  int * out = outputInt->get(); 
  *out = stepno;
  
  log("producing "+to_string(stepno));
  sleep(500);
  
  if(stepno == last) {
    outputInt->setStatus(TokenStatus::EOS);
    status = ActorStatus::EOS;
  }
  
  outputInt->unlock();
  
}

IntProducer::~IntProducer() {
  destroyPort(outputInt);
}
