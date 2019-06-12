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

#ifndef DF_HAMILTONIAN_H_
#define DF_HAMILTONIAN_H_

#include "core/df.h"
#include "core/arithmatic.h"
#include "tokens/basic/msg.h"

#include <algorithm>
#include <string>
#include <iostream>

class Hamiltonian: public df::Actor {
  
private:
  df::InputPortVector<df::Msg> * input;
  df::OutputPortVector<df::Msg> * output;

  df::Arithmatic arith;
  
  std::string imsg, omsg;
  std::string input_messages;
  std::string output_message;
  int nbnodes;
  int prime;
  bool first;
  static df::ActorRegister<Hamiltonian> reg;
public:
  
  Hamiltonian(const std::string & name);
  
  virtual void init();
  virtual void run();
  
  virtual ~Hamiltonian();
  
};

#endif /* DF_HAMILTONIAN_H_ */
