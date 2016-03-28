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

#include "filters/basic/AdditionFilter.h"

using namespace tmf;
using namespace std;

FilterRegister<AdditionFilter> AdditionFilter::reg("addition");

AdditionFilter::AdditionFilter(const string & name) :
Filter(name) {
  input1 = createInputPort<int>("int input 1");
  input2 = createInputPort<int>("int input 2");
}


void  AdditionFilter::run() {
  
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

AdditionFilter::~AdditionFilter() {
  destroyPort(input1);
  destroyPort(input2);
}

