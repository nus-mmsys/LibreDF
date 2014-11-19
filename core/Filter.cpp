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

Filter::Filter(const string &name) : realtime(false), status(FilterStatus::OK), name(name) {
  inMsg = nullptr;
  outMsg = new Message();
}

void Filter::setRealTime(bool rt) {
  realtime = rt;
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
  t = new thread(&Filter::initFilter, this);
}

void Filter::startRun() {
  t = new thread(&Filter::runFilter, this);
}

void Filter::wait() {
  t->join();
  
  delete t;
}

void Filter::setIOLock(mutex * mux) {

  io_lock = mux;
}

void Filter::initFilter() {
  init();
}

void Filter::runFilter() {
  
  while(status != FilterStatus::EOS) {
    realtime? runRT() : run();
  }
  
}

Filter::~Filter() { 
  delete outMsg;
}

//void Filter::setProp(const string & key, const string & val) {
//  props.emplace(this->name + "::" + key, val);
//}

//string Filter::getProp(const string & key) {
//  return props[this->name + "::" + key];
//}

/*
 * void Filter::initFilter(Message * msg) {
 *  void status = FILTER_SUCCESS;
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


//int Filter::inputPortNum() {
//  return inputPorts.size();
//}

//int Filter::outputPortNum() {
//  return outputPorts.size();
//}

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
