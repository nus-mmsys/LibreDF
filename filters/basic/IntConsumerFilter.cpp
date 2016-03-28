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

#include "filters/basic/IntConsumerFilter.h"

using namespace tmf;
using namespace std;

FilterRegister<IntConsumerFilter> IntConsumerFilter::reg("int_consumer");

IntConsumerFilter::IntConsumerFilter(const string & name) : Filter(name) {
  input = createInputPort<int>("input");
}

void IntConsumerFilter::run() {
  
  input->lock();
  
  int * inputData = input->get();
  
  log("consuming "+to_string(*inputData));
  sleep(500);
  
  if (input->getStatus() == SampleStatus::EOS)
    status = FilterStatus::EOS; 
  
  input->unlock();
  
}

IntConsumerFilter::~IntConsumerFilter() {
  destroyPort(input);
}
