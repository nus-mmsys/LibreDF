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

#ifndef DF_HAMILTON_H_
#define DF_HAMILTON_H_

#include "core/df.h"
#include "tokens/basic/str.h"

#include <algorithm>
#include <string>
#include <iostream>

class Hamilton: public df::Actor {
  
private:
  df::InputPortVector<df::Str> * input;
  df::OutputPortVector<df::Str> * output;

  std::string imsg, omsg;
  std::string input_messages;
  std::string output_message;
  int nbnodes;
  static df::ActorRegister<Hamilton> reg;
public:
  
  Hamilton(const std::string & name);
  
  virtual void init();
  virtual void start();
  virtual void run();
  
  virtual ~Hamilton();
  
};

#endif /* DF_HAMILTON_H_ */
