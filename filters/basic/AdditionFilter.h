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
  
public:
  
  AdditionFilter(const string & name) :
  Filter(name) {
    input1 = new InputPort<int>("int input 1");
    input2 = new InputPort<int>("int input 2");
    
    inputPorts.push_back(input1);
    inputPorts.push_back(input2);
  }
  
  
  void run() {
    
    input1->lock();
    input2->lock();
    
    int * inputData1 = input1->get();
    
    int * inputData2 = input2->get();
    
    int outputint = *inputData1 + *inputData2;
    
    log("addition "+to_string(outputint));
    
    if (input1->getStatus() == SampleStatus::EOS ||
      input2->getStatus() == SampleStatus::EOS)
      status = FilterStatus::EOS;
    
    input1->unlock();
    input2->unlock();
  }
  
  ~AdditionFilter() {
    delete input1;
    delete input2;
  }
  
};

#endif /* ADDITIONFILTER_H_ */
