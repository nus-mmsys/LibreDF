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

#ifndef DF_REMOTE_H_
#define DF_REMOTE_H_

#include "core/df.h"

#include <iostream>

class Remote: public df::Actor {
  
private:
  static df::ActorRegister<Remote> reg;
public:
  
  Remote(const std::string & name): df::Actor(name) {}
  virtual void init() {}
  virtual void run() {}
  virtual ~Remote() {}
  
};

#endif /* DF_REMOTE_H_ */
