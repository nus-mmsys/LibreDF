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

#include "Pipeline.h"

using namespace tmf;
using namespace std;

Pipeline::Pipeline(const string& name): name(name), realtime(false), status(PipelineStatus::STOPPED) {
}


void Pipeline::setRealTime(bool rt) { 
  realtime = rt;
  
}

void Pipeline::addFilter(Filter * f) {
  bus.registerObserver(f);
  f->setBusRef(&bus);
  f->setPipeLock(&io_lock);
  filters.insert(f);
}

void Pipeline::addFilters(Filter * f, ...) {
  
  va_list arguments;                     
  
  Filter * cur;
  va_start ( arguments, f);
  
  cur = f;
  while(cur != nullptr) {
    
    addFilter(cur);
    cur = va_arg(arguments, Filter*);
  }
  va_end ( arguments ); 
  
}

void Pipeline::connectFilters(Filter * inf, Filter * outf) {
  
  inf->connectFilter(outf);
}


void Pipeline::init() {
  
  for (auto f : filters) {
    f->setRealTime(realtime);
  }
  
  for (auto f: filters) {
    f->startInit();
  }
  
  for (auto f : filters) {
    f->waitInit();
  }
  
  status = PipelineStatus::READY;
  
  for (auto f : filters) {
    
    if (f->getStatus() != FilterStatus::OK)
      status = PipelineStatus::STOPPED;
    return;
  }
  
  
}

void Pipeline::run() {
  
  if (status != PipelineStatus::READY) {
    std::cout << "Pipeline is not ready to run." << endl;
    return;
  }
  
  for (auto f : filters) {
    f->startRun();
  }
  
  status = PipelineStatus::RUNNING;
  
  for (auto f : filters) {
    f->waitRun();
  }
  
  status = PipelineStatus::STOPPED;
  
}

Pipeline::~Pipeline() {
  
  for (auto f : filters)
    if(f)
      delete f;
    
}
