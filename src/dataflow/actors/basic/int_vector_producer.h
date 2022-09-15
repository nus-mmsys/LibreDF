/*
 *   LibreDF
 *   Copyright (C) A. Shafiei
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

#ifndef DF_INTVECTORPRODUCER_H_
#define DF_INTVECTORPRODUCER_H_

#include "core/df.h"
#include "tokens/basic/int_vector.h"

#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

class IntVectorProducer: public df::Actor {
  
private:

  std::string msg;

  int step;
  int number;  
  int last;
  
  df::OutputPort<df::IntVector> * output;
 
  static  df::ActorRegister<IntVectorProducer> reg;
public:
  
  IntVectorProducer(const string& name);
  
  virtual void init();
  
  virtual void run();
  
  virtual ~IntVectorProducer();
  
};

#endif /* DF_INTVECTORPRODUCER_H_ */
