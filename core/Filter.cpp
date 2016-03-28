/*
 * 
 *  Tiny Multimedia Framework (tmf)
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This file is part of tmf.
 *
 *  tmf is free software; you can redistribute it and/or 
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  tmf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with tmf.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "core/Filter.h"

using namespace tmf;
using namespace std;

Filter::Filter(const string &name) : realtime(false), status(FilterStatus::OK), name(name) {
  
}

void Filter::setRealTime(bool rt) {
  realtime = rt;
}

void Filter::log(std::string msg) {
  pipelock->lock(); 
  std::cout << name << ": " << msg << std::endl;
  pipelock->unlock();
}
void Filter::sleep(int s) {
  this_thread::sleep_for(chrono::milliseconds{rand()%s});
}
void Filter::connectFilter(Filter * f) {
  
  bool linked = false;
  for (auto fout : outputPorts) {
    for (auto fin : f->inputPorts) {
      
      const PortCaps& typeOut = fout->getPortCaps();
      const PortCaps& typeIn = fin->getPortCaps();
      
      if ( fin->getLinked() == 0 && (typeOut.isEqual(typeIn))) {
	
	fout->connectPort(fin);
	
	linked = true;
	break;
      }
    }
    if (linked)
      break;
  }
}

void Filter::startInit() {
  tinit = thread(&Filter::initFilter, this);
}

void Filter::startRun() {
  trun = thread(&Filter::runFilter, this);
}

void Filter::waitInit() {
  tinit.join();
}

void Filter::waitRun() {
  trun.join();
}

void Filter::setPipeLock(mutex * mux) {
  pipelock = mux;
}

void Filter::initFilter() {

  for (auto p : inputPorts) {
    if (p->getLinked() == 0) {
      log(p->getName()+string(" is not connected"));
      status = FilterStatus::ERROR;
    }
  }
  
  for (auto p : outputPorts) {
    if (p->getLinked() == 0) {
      log(p->getName()+string(" is not connected"));
      status = FilterStatus::ERROR;
    }
  } 
  init();
}

void Filter::runFilter() {
  
  while(status != FilterStatus::EOS) {
    realtime? runRT() : run();
  }
  
}

void Filter::handleEvent(Event event) {
  
  if (event.type == EventType::EOS) {
    
    status = FilterStatus::EOS;
  }
}

void Filter::sendEvent(Event event) {
  
  pipelock->lock();
  busref->notify(event);
  pipelock->unlock();
}

Filter::~Filter() { 
}
