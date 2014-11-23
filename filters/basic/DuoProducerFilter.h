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

#ifndef DUOPRODUCERFILTER_H_
#define DUOPRODUCERFILTER_H_

#include "core/Filter.h"
#include "core/Port.h"
#include "core/tmf.h"

#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

class DuoProducerFilter: public Filter {
  
private:
  
  int limit;
  
  OutputPort<int> * outputInt;
  OutputPort<string> * outputString;
  
  static  FilterRegister<DuoProducerFilter> reg;
public:
  
  DuoProducerFilter(const string& name);
  
  virtual void init();
  
  virtual void run();
  
  virtual ~DuoProducerFilter();
  
};

#endif /* DUOPRODUCERFILTER_H_ */
