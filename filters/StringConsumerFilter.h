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

#ifndef STRINGCONSUMERFILTER_H_
#define STRINGCONSUMERFILTER_H_

#include "core/Port.h"
#include "core/Filter.h"
#include <unistd.h>
#include <iostream>

struct StringConsumerFilter: public Filter {
  
private:
  InputPort<std::string> * input;
public:
  
  StringConsumerFilter(const string & name) : Filter(name) {
    input = new InputPort<std::string>("input string");
    
    inputPorts.push_back(input);
  }
  
  FilterStatus run() {
    
    FilterStatus status = FILTER_SUCCESS;
    
    input->lock();
    
    string * inputData = input->get();
    
    log("consuming "+*inputData);
    sleep(500);
   
    if (input->getStatus() == SampleStatus::EOS)
      status = FILTER_FINISHED; 
    
    input->unlock();
    
    return status;
  }
  
  ~StringConsumerFilter() {
    delete input;
  }
  
};

#endif /* STRINGCONSUMERFILTER_H_ */
