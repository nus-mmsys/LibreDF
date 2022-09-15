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

#ifndef DF_DELAY_ACTOR_C_H_
#define DF_DELAY_ACTOR_C_H_

#include "delay_actor.h"

class DelayActorC: public DelayActor {
  
private:
  static df::ActorRegister<DelayActorC> reg;
public:
  DelayActorC(const std::string & name);
};

#endif /* DF_DELAY_ACTOR_C_H_ */
