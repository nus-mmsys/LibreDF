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

#ifndef DF_DOUBLINGMR_H_
#define DF_DOUBLINGMR_H_

#include "core/df.h"
#include "tokens/basic/int.h"

class DoublingMR: public df::Actor {
private:
  df::InputPort<df::Int> * input;
  df::OutputPort<df::Int> * output;
  int outval;  
  static df::ActorRegister<DoublingMR> reg;
public:
  
  DoublingMR(const std::string & name);
  
  virtual void init();
  virtual void run();
  
  virtual ~DoublingMR();
  
};

#endif /* DOUBLINGMR_H_ */
