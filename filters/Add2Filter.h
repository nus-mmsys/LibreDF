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

#ifndef ADD2FILTER_H_
#define ADD2FILTER_H_

#include "core/Filter.h"
#include "core/Port.h"

struct Add2Filter: public Filter {
private:
  InputPort<int> * input;
  OutputPort<int> * output;
public:
  
  Add2Filter(const string & name) :
  Filter(name) {
    input = new InputPort<int>("add2, input 1, int");
    output = new OutputPort<int>("add2, output 1, int");
    
    inputPorts.push_back(input);
    outputPorts.push_back(output);
  }
  
  void run() {
    
    input->lock();
    
    int * inputData = input->get();
    
    int outputint = *inputData + 2;
    
    input->unlock();
    
    output->lock();
    
    int * outputData =  output->get();
    *outputData = outputint;
    
    output->unlock();
    
  }
  
  ~Add2Filter() {
    delete input;
    delete output;
  }
  
};

#endif /* ADD2FILTER_H_ */
