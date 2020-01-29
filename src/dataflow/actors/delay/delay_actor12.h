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

#ifndef DF_DELAY_ACTOR12_H_
#define DF_DELAY_ACTOR12_H_

#include "core/df.h"
#include "tokens/basic/int.h"

#include <iostream>
#include <string>

class DelayActor12: public df::Actor {
  
private:

  df::InputPort<df::Int> * input;
  df::OutputPort<df::Int> * output1;
  df::OutputPort<df::Int> * output2;

  int delay;
  int counter, max;

  static df::ActorRegister<DelayActor12> reg;
public:
  
  DelayActor12(const std::string & name);
  virtual void init();
  virtual void reinit();
  virtual void run();
  virtual ~DelayActor12();
  
};

#endif /* DF_DELAY_ACTOR12_H_ */
