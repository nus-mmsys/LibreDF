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

#ifndef DF_HAMILTON2_H_
#define DF_HAMILTON2_H_

#include "core/df.h"
#include "tokens/basic/str.h"

#include <algorithm>
#include <string>
#include <iostream>

class Hamilton2: public df::Actor {
  
private:
  df::InputPortVector<df::Str> * input;
  df::OutputPortVector<df::Str> * output;

  std::string msg;
  std::string loop;
  std::string input_messages;
  std::string output_message;
  std::vector<std::string> * loop_list;
  int nbnodes;
  bool first;
  static df::ActorRegister<Hamilton2> reg;
  bool hamiltonian(const std::string & msg);
  int path_length(const std::string & msg);
public:
  
  Hamilton2(const std::string & name);
  
  virtual void init();
  virtual void run();
  
  virtual ~Hamilton2();
  
};

#endif /* DF_HAMILTON2_H_ */
