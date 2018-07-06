/*
 *   libdataflow
 *   Copyright (C) 2018
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DF_INTPRODUCER_H_
#define DF_INTPRODUCER_H_

#include "core/df.h"
#include "tokens/basic/int.h"

#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

class IntProducer: public df::Actor {
  
private:
  
  int last;
  
  df::OutputPort<df::Int> * output;
 
  static  df::ActorRegister<IntProducer> reg;
public:
  
  IntProducer(const string& name);
  
  virtual void init();
  
  virtual void run();
  
  virtual void runRT();
  //virtual void runDist();
  
  virtual ~IntProducer();
  
};

#endif /* DF_INTPRODUCER_H_ */
