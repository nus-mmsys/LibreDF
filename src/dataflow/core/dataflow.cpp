/*
 *   libdataflow
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

#include "dataflow.h"

using namespace df;
using namespace std;

Dataflow::Dataflow(const string& name): name(name), status(DataflowStatus::STOPPED) {
	realtime = false;
	distributed = false;
}

void Dataflow::addActor(Actor * f) {
  f->setPipeLock(&io_lock);
  actors.insert(f);
}

void Dataflow::addActors(Actor * f, ...) {
  
  va_list arguments;                     
  
  Actor * cur;
  va_start ( arguments, f);
  
  cur = f;
  while(cur != nullptr) {
    
    addActor(cur);
    cur = va_arg(arguments, Actor*);
  }
  va_end ( arguments ); 
  
}

void Dataflow::connectActors(Actor * inf, Actor * outf) {
  
  inf->connectActor(outf);
}

void Dataflow::connectActors(Actor * in, Actor * out, int p, int c) {
    
  in->connectActor(out, p, c);
}

void Dataflow::init() {
  
  if (!prop.propEmpty("distributed"))
	  distributed = prop.getPropBool("distributed");

  if (!prop.propEmpty("realtime"))
	  realtime = prop.getPropBool("realtime");
	
  for (auto f : actors) {
    f->setRealTime(realtime);
  }
  
  for (auto f: actors) {
    f->startInit();
  }
  
  for (auto f : actors) {
    f->waitInit();
  }
  
  status = DataflowStatus::READY;
  
  for (auto f : actors) {
    
    if (f->getStatus() != ActorStatus::OK)
      status = DataflowStatus::STOPPED;
    return;
  }
  
  
}

void Dataflow::run() {
  
  if (status != DataflowStatus::READY) {
    std::cout << "Dataflow is not ready to run." << endl;
    return;
  }
  
  for (auto f : actors) {
    f->startRun();
  }
  
  status = DataflowStatus::RUNNING;
  
  for (auto f : actors) {
    f->waitRun();
  }
  
  status = DataflowStatus::STOPPED;
  
}

Dataflow::~Dataflow() {
  
  for (auto f : actors)
    if(f)
      delete f;
    
}
