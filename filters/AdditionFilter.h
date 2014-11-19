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

#ifndef ADDITIONFILTER_H_
#define ADDITIONFILTER_H_

#include "core/Filter.h"
#include <Port.h>
#include <iostream>

struct AdditionFilter: public Filter {
  
private:
  InputPort<int> * input1;
  InputPort<int>* input2;
  OutputPort<int> * output;
  
public:
  
  AdditionFilter(const string & name) :
  Filter(name) {
    input1 = new InputPort<int>("addition, input 1, int");
    input2 = new InputPort<int>("addition, input 2, int");
    output = new OutputPort<int>("addition, output 1, int");
    
    inputPorts.push_back(input1);
    inputPorts.push_back(input2);
    outputPorts.push_back(output);
  }
  
  
  void run() {
    
    
    int * inputData1 = input1->get();
    
    int * inputData2 = input2->get();
    
    int outputint = *inputData1 + *inputData2;
    
    output->lock();
    
    int * outputData = output->get();
    
    *outputData = outputint;
    
    output->unlock();
    
    cout << "Addition= " << outputint << endl;
    
  }
  
  ~AdditionFilter() {
    delete input1;
    delete input2;
    delete output;
  }
  
};

#endif /* ADDITIONFILTER_H_ */
