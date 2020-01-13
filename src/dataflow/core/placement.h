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

#ifndef DF_PLACEMENT_H_
#define DF_PLACEMENT_H_

#include <map>
#include <vector>
#include <string>

#include "actor.h"

enum PlacementPolicy {
	ROUND_ROBIN,
	SINGLE_PROCESSOR,
	DEFAULT
};

namespace df {
	
  class Placement {
	private:
	  int cpunb;
	public:
	  Placement();
	  void place(const std::vector<df::Actor *> & actors, PlacementPolicy policy, int init);
	  void place(const std::map<std::string, df::Actor *> & actors, PlacementPolicy policy, int init);
	  void roundRobin(const std::vector<df::Actor *> & actors, int init);
	  void singleProcessor(const std::vector<Actor *> & actors);
  };

}

#endif /* DF_PLACEMENT_H_ */
