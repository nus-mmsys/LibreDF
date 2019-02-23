/*
 *   TMF
 *   Copyright (C) TMF Team
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

#ifndef DF_INTCONSUMERMR_H_
#define DF_INTCONSUMERMR_H_

#include "core/df.h"
#include "tokens/basic/int.h"

#include <iostream>

class IntConsumerMR: public df::Actor {
  
private:
  df::InputPort<df::Int> * input;

  static df::ActorRegister<IntConsumerMR> reg;
public:
  
  IntConsumerMR(const std::string & name);
  
  virtual void init();
  virtual void run();
  
  virtual ~IntConsumerMR();
  
};

#endif /* DF_INTCONSUMERMR_H_ */
