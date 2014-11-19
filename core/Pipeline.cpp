/*
 * 
 *  Tiny Multimedia Framework
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "Pipeline.h"

Pipeline::Pipeline(const string& name): name(name), realtime(false), status(PipelineStatus::STOPPED) {
}


void Pipeline::setRealTime(bool rt) { 
  realtime = rt;

}

void Pipeline::connectFilters(Filter * inf, Filter * outf) {
  
  inf->setIOLock(&io_lock);
  outf->setIOLock(&io_lock);
  
  filters.insert(inf);
  filters.insert(outf);
  
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
    f->wait();
  }
  
}

void Pipeline::run() {
  for (auto f : filters) {
    f->startRun();
  }
  
  for (auto f : filters) {
    f->wait();
  }
  
}

Pipeline::~Pipeline() {
  
  for (auto f : filters)
    delete f;
  
}
