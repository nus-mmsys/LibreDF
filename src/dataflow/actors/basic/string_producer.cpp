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

#include "string_producer.h"

using namespace df;
using namespace std;

ActorRegister<StringProducer> StringProducer::reg("StringProducer");

StringProducer::StringProducer(const string& name) : Actor(name){
  output = createOutputPort<Str>("string output");
}

void StringProducer::init() {
  if (getProp("last") != "")  	
  	last = stoi(getProp("last"));
  else
        last = 100;
}

void StringProducer::run() {

  Str * out = produce(output);
  out->set(to_string(stepno));
  log("producing "+*out->get());
  sleep(200);

  if(stepno == last)
    setEos(output);

  release(output);

}

void StringProducer::runRT() {
  
  string data = to_string(stepno);
  
  bool canlock = output->lockRT();
  
  if (!canlock) {
    log("droping "+data);
    sleep(200);
    return;
  }
  
  Str * outStr = output->get(); 
  outStr->set(data);
  
  log("producing "+*outStr->get());
  sleep(200);
  
  if(stepno == last)
    setEos(output);
  
  output->unlock();
  
}

StringProducer::~StringProducer() {
  destroyPort(output);
}
