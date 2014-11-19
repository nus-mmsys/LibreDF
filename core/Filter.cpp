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

#include "Filter.h"
#include "Port.h"

#include <iostream>

Filter::Filter(const string &name) {
  
  this->name = name;
  linked = 0;
  inputFed = 0;
  inMsg = 0;
  outMsg = new Message();
  
}

void Filter::log(std::string msg) {
  io_lock->lock(); 
  std::cout << name << ": " << msg << std::endl;
  io_lock->unlock();
}
void Filter::sleep(int s) {
  this_thread::sleep_for(chrono::milliseconds{rand()%s});
}
void Filter::connectFilter(Filter * f) {
  
  vector<Port*>::iterator itOut;
  vector<Port*>::iterator itIn;
  
  bool linked = false;
  for (itOut = outputPorts.begin(); itOut != outputPorts.end();++itOut) {
    for (itIn = f->inputPorts.begin(); itIn != f->inputPorts.end();++itIn) {
      
      const PortCaps& typeOut = (*itOut)->getPortCaps();
      const PortCaps& typeIn = (*itIn)->getPortCaps();
      if ( (*itIn)->getLinked() == 0 && (typeOut.isEqual(typeIn))) {
	
	(*itOut)->connectPort(*itIn);
	
	//addNextFilter((*itOut), f);
	
	linked = true;
	break;
      }
    }
    if (linked)
      break;
  }
}

void Filter::setProp(const string & key, const string & val) {
  props.emplace(this->name + "::" + key, val);
}

string Filter::getProp(const string & key) {
  return props[this->name + "::" + key];
}

FilterStatus Filter::run() {
  FilterStatus status = FILTER_SUCCESS;
  
  while(status != FILTER_FINISHED) {
    //if (linked > 0 && inputFed + 1 != linked) {
    //  inputFed++;
    //  return FILTER_WAIT_FOR_INPUT;
    //}
    
    //inputFed = 0;
    status = process();
  }
  return status;
}

/*
 * FilterStatus Filter::initFilter(Message * msg) {
 *  FilterStatus status = FILTER_SUCCESS;
 *  
 *  inMsg = msg;
 *  
 *  status = init();
 *  
 *  if (status == FILTER_WAIT_FOR_INPUT)
 *    return FILTER_WAIT_FOR_INPUT;
 *  
 *  vector<OutputPort*>::iterator itIn;
 *  for (itIn = outputPorts.begin(); itIn != outputPorts.end(); ++itIn) {
 *    
 *    Port * curPort = (*itIn);
 *    initNextFilters(curPort, outMsg);
 *    
 *  }
 *  
 *  return status;
 * }
 */

void Filter::increaseLinked() {
  linked++;
}

int Filter::inputPortNum() {
  return inputPorts.size();
}

int Filter::outputPortNum() {
  return outputPorts.size();
}

/* void Filter::processNextFilters(Port * p) {
 *	vector<Filter*> * nextFilters = getNextFilters(p);
 *	vector<Filter*>::iterator itNxt;
 *	for (itNxt = nextFilters->begin(); itNxt != nextFilters->end(); ++itNxt) {
 *		(*itNxt)->executeFilter();
 *	}
 * }
 */

/* void Filter::initNextFilters(Port *p, Message * msg) {
 *	vector<Filter*> * nextFilters = getNextFilters(p);
 *	if (nextFilters) {
 * 
 *		for (auto i : *nextFilters) {
 *			i->initFilter(msg);
 *		}
 *		//for (vector<Filter*>::iterator itNxt = nextFilters->begin(); itNxt != nextFilters->end();
 *		//		++itNxt) {
 *		//	(*itNxt)->initFilter(msg);
 *		//}
 *	}
 * }
 */

/* vector<Filter*> * Filter::getNextFilters(Port *p) {
 *	return this->nextFilters[p];
 * }
 */

/*void Filter::addNextFilter(Port * p, Filter *f) {
 *	std::map<Port*, vector<Filter*>*>::iterator it;
 * 
 *	it = this->nextFilters.find(p);
 * 
 *	vector<Filter*> * nf;
 *	if (it == this->nextFilters.end()) {
 *		nf = new vector<Filter*>();
 *		this->nextFilters.emplace(p, nf);
 *	} else {
 *		nf = getNextFilters(p);
 *	}
 * 
 *	nf->push_back(f);
 * 
 *	f->increaseLinked();
 * 
 * }
 */

void Filter::start() {
  t = new thread(&Filter::run, this);
}

void Filter::startRT() {
  t = new thread(&Filter::run, this);
}

void Filter::wait() {
  t->join();
}

void Filter::setIOLock(mutex * mux) {

  io_lock = mux;
}

Filter::~Filter() {
  
  delete outMsg;
  
}
