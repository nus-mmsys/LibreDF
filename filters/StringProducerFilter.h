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

#ifndef STRINGPRODUCERFILTER_H_
#define STRINGPRODUCERFILTER_H_ 

#include "core/Filter.h"
#include "core/Port.h"

#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

class StringProducerFilter: public Filter {
  
private:
 
  int number;
  OutputPort<string> * outputString;
  
public:
  
  StringProducerFilter(const string& name) : Filter(name), number(0) {
    
    outputString = new OutputPort<string>(
      "string output");
    
    outputPorts.push_back(outputString);
  }
  
  FilterStatus process() {
    
    outputString->lock();
    
    string * outStr = outputString->get(); 
    *outStr = to_string(number);
    
    log("producing "+*outStr);
    sleep(500);
    
    outputString->unlock();
    
    number++;
    
    return FILTER_SUCCESS;
  }
  
  ~StringProducerFilter() {
    delete outputString;
  }
  
};

#endif /* STRINGPRODUCERFILTER_H_ */
