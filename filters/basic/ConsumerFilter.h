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

#ifndef CONSUMERFILTER_H_
#define CONSUMERFILTER_H_

#include "core/Port.h"
#include "core/Filter.h"
#include <unistd.h>
#include <iostream>
#include <sstream>

template <typename T>
class ConsumerFilter: public Filter {
  
private:
  InputPort<T> * input;
public:
  
  ConsumerFilter(const string & name) : Filter(name) {
    input = new InputPort<T>("input");
    
    inputPorts.push_back(input);
  }
  
  void run() {
    
    input->lock();
    
    T * inputData = input->get();
    
    std::stringstream inputstr;
    inputstr << "consuming " << *inputData;
    
    log(inputstr.str());
    sleep(500);
    
    if (input->getStatus() == SampleStatus::EOS)
      status = FilterStatus::EOS; 
    
    input->unlock();
    
  }
  
  ~ConsumerFilter() {
    delete input;
  }
  
};

#endif /* CONSUMERFILTER_H_ */
