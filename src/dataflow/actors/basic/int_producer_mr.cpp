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

#include "int_producer_mr.h"

using namespace df;
using namespace std;

ActorRegister<IntProducerMR> IntProducerMR::reg("IntProducerMR");

IntProducerMR::IntProducerMR(const string& name) : Actor(name) {
  output = createOutputPort<Int>("output");
}

void IntProducerMR::init() {
 
  if (!propEmpty("last"))  	
  	last = getPropInt("last");
  else
        last = 100;
  
}

void IntProducerMR::run() {

  vector<Int *> out = produceMR(output);	
  int index = 1;
  
  log("producing ");
  for (auto t : out) {
        log(" "+to_string(stepno*index));
  	t->set(stepno*index);
	index = index * 10;
  }

  timer.randSleep(200);

  if(stepno == last)
    setEos(output);

  releaseMR(output);  

}

/*
void IntProducerMR::runDist() {

  char buf[1024];
  memset(buf, 0, 1024);
  strcpy(buf,(to_string(stepno)).c_str());
  send(output, buf);
  log("producing "+to_string(stepno));
  timer.randSleep(200);

}
*/

void IntProducerMR::runRT() {
  
  string data = to_string(stepno);
  
  bool canlock = output->lockRT();
  
  if (!canlock) {
    log("droping "+data);
    timer.randSleep(200);
    return;
  }
  
  Int * out = output->get(); 
  out->set(stepno);
  
  log("producing "+to_string(stepno));
  timer.randSleep(200);
  
  if(stepno == last)
    setEos(output);
  
  output->unlock();
  
}

IntProducerMR::~IntProducerMR() {
  destroyPort(output);
}
