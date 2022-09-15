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

#ifndef DF_VOID_ACTOR_H_
#define DF_VOID_ACTOR_H_

#include "core/df.h"
#include "tokens/basic/int.h"

#include <iostream>
#include <string>

class VoidActor: public df::Actor {
  
private:

  df::InputPort<df::Int> * input;
  df::OutputPort<df::Int> * output;

  static df::ActorRegister<VoidActor> reg;
public:
  
  VoidActor(const std::string & name);
  virtual void init();
  virtual void reinit();
  virtual void run();
  virtual ~VoidActor();
  
};

#endif /* DF_VOID_ACTOR_H_ */
