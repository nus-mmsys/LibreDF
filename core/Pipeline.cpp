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

Pipeline::Pipeline(const string& name): name(name), status(PIPELINE_STOPPED) {
}


void Pipeline::setRealTime(bool rt) { 
  for (auto f : filters) {
    f->setRealTime(rt);
  }
}

void Pipeline::connectFilters(Filter * inf, Filter * outf) {
  
  inf->setIOLock(&io_lock);
  outf->setIOLock(&io_lock);
  
  filters.insert(inf);
  filters.insert(outf);
  
  inf->connectFilter(outf);
}


PipelineStatus Pipeline::init() {
  
  for (auto f: filters) {
    
    f->startInit();
    /*if (ret == FILTER_ERROR) {
      cerr << "Pipeline cannot initialize a filter.\n";
      return PIPELINE_STOPPED;
    }*/
  }
  
  for (auto f : filters) {
    f->wait();
  }
  
  return PIPELINE_RUNNING;
  
}

PipelineStatus Pipeline::run() {
  for (auto f : filters) {
    f->startRun();
  }
  
  for (auto f : filters) {
    f->wait();
  }
 
  /*
   *	FilterStatus status;
   *	while(1) {
   * 
   *		status = start->run();
   * 
   *		switch(status) {
   *		case FILTER_SUCCESS:
   *		case FILTER_WAIT_FOR_INPUT:
   *			continue;
   *		case FILTER_ERROR:
   *			return PIPELINE_STOPPED;
   *		case FILTER_FINISHED:
   *			return PIPELINE_FINISHED;
   * 
}

}
*/
  return PIPELINE_FINISHED;
  
}

Pipeline::~Pipeline() {
  
  for (auto f : filters)
    delete f;
  
}
