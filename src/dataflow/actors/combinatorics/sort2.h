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

#ifndef DF_SORT2_H_
#define DF_SORT2_H_

#include "core/df.h"
#include "tokens/basic/int_vector.h"

#include <iostream>
#include <string>
#include <unistd.h>
#include <cmath>

using namespace std;

class Sort2: public df::Actor {
  
private:
  
  df::InputPort<df::IntVector> * input;
  df::OutputPort<df::IntVector> * output;
 
  static  df::ActorRegister<Sort2> reg;
public:
  
  Sort2(const string& name);
  
  virtual void init();
  
  virtual void run();
  
  virtual ~Sort2();
  
};

#endif /* DF_SORT2_H_ */
