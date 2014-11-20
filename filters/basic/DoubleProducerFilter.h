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

#ifndef DOUBLEPRODUCERFILTER_H_
#define DOUBLEPRODUCERFILTER_H_

#include "core/Filter.h"
#include "core/Port.h"

#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

class DoubleProducerFilter: public Filter {
  
private:
  
  int limit;
  
  OutputPort<int> * outputInt;
  OutputPort<string> * outputString;
  
public:
  
  DoubleProducerFilter(const string& name) :
  Filter(name) {
    outputInt = new OutputPort<int>("int output");
    outputString = new OutputPort<string>(
      "string output");
    
    outputPorts.push_back(outputInt);
    outputPorts.push_back(outputString);
  }
  
  void init() {
    limit = stoi(getProp("limit")); 
  }
  
  void run() {
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
  
  ~DoubleProducerFilter() {
    delete outputInt;
    delete outputString;
  }
  
};

#endif /* DOUBLEPRODUCERFILTER_H_ */
