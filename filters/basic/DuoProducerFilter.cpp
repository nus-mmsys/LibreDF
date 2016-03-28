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

#include "filters/basic/DuoProducerFilter.h"

using namespace tmf;
using namespace std;

FilterRegister<DuoProducerFilter> DuoProducerFilter::reg("duo_producer");

DuoProducerFilter::DuoProducerFilter(const string& name) :
Filter(name) {
  outputInt = createOutputPort<int>("int output");
  outputString = createOutputPort<string>("string output");
}

void DuoProducerFilter::init() {
  limit = stoi(getProp("limit")); 
}

void DuoProducerFilter::run() {
  static int number = 1;
  
  outputInt->lock();
  int * outInt = outputInt->get();
  *outInt = number;
  
  if(number == limit) {
    outputInt->setStatus(SampleStatus::EOS);
    status = FilterStatus::EOS;
  }
  outputInt->unlock();
  
  outputString->lock();
  string * outStr = outputString->get(); 
  *outStr = to_string(number);
  log("producing "+*outStr);
  
  if(number == limit) {
    outputString->setStatus(SampleStatus::EOS);
  }
  outputString->unlock();
  
  number++;
}

DuoProducerFilter::~DuoProducerFilter() {
  destroyPort(outputInt);
  destroyPort(outputString);
}
