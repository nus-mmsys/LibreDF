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

#include "filters/basic/StringProducerFilter.h"

using namespace tmf;
using namespace std;

FilterRegister<StringProducerFilter> StringProducerFilter::reg("string_producer");

StringProducerFilter::StringProducerFilter(const string& name) : Filter(name), number(0) {
  outputString = createOutputPort<string>("string output");
}

void StringProducerFilter::init() {
  limit = stoi(getProp("limit"));
}

void StringProducerFilter::run() {
  
  outputString->lock();
  
  string * outStr = outputString->get(); 
  *outStr = to_string(number);
  
  log("producing "+*outStr);
  sleep(500);
  
  if(number == limit) {
    outputString->setStatus(SampleStatus::EOS);
    status = FilterStatus::EOS;
  }
  
  outputString->unlock();
  
  number++;
}

void StringProducerFilter::runRT() {
  
  string data = to_string(number);
  
  bool canlock = outputString->lockRT();
  
  if (!canlock) {
    log("droping "+data);
    sleep(500);
    number++;
    return;
  }
  
  string * outStr = outputString->get(); 
  *outStr = data;
  
  log("producing "+*outStr);
  sleep(500);
  
  if(number == limit) {
    outputString->setStatus(SampleStatus::EOS);
    status = FilterStatus::EOS;
  }
  
  outputString->unlock();
  
  number++;
}

StringProducerFilter::~StringProducerFilter() {
  destroyPort(outputString);
}
