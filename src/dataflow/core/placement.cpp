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

#include "placement.h"

using namespace std;
using namespace df;

Placement::Placement() {
  cpunb = std::thread::hardware_concurrency();
}

void Placement::place(const std::map<std::string, Actor *> & actmap, int policy, int init) {
	std::vector<Actor *> actors;
	for (auto ac : actmap)
		actors.push_back(ac.second);
	place(actors, policy, init);
}

void Placement::place(const std::vector<Actor *> & actors, int policy, int init) {
	switch (policy) {
		case 0:
			roundRobin(actors, init);
			break;
		case 1:
			singleProcessor(actors);
			break;
		case 2:
			manual(actors);
			break;
		case 3:
			manual3(actors);
			break;
		case 4:
			manual4(actors);
			break;
		default:
			roundRobin(actors, init);
			break;
	}
}

void Placement::roundRobin(const std::vector<Actor *> & actors, int init) {

  int cpuid = init % cpunb;

  for (auto ac : actors) {
    ac->setCpuId(cpuid);
    cpuid = (cpuid + 1) % cpunb;
  }

}

void Placement::singleProcessor(const std::vector<Actor *> & actors) {

  int cpuid = 0;

  for (auto ac : actors) {
    ac->setCpuId(cpuid);
  }

}

void Placement::manual(const std::vector<Actor *> & actors) {

  for (auto ac : actors) {
    if (ac->getName() == "Src" || ac->getName() == "S")
    	ac->setCpuId(cpunb - 1);
    else if (ac->getName() == "Snk" || ac->getName() == "J")
    	ac->setCpuId(cpunb - 2);
    else if (ac->getName() == "A1" || ac->getName() == "B1" 
		    || ac->getName() == "C1")
    	ac->setCpuId(cpunb - 3);
    else
    	ac->setCpuId(cpunb - 4);
  }
}

void Placement::manual3(const std::vector<Actor *> & actors) {

  for (auto ac : actors) {
    if (ac->getName() == "Src" || ac->getName() == "S"
        || ac->getName() == "Snk" || ac->getName() == "J")
    	ac->setCpuId(cpunb - 1);
    else if (ac->getName() == "A1" || ac->getName() == "B1" 
		    || ac->getName() == "C1")
    	ac->setCpuId(cpunb - 2);
    else if (ac->getName() == "A2" || ac->getName() == "B3" 
		    || ac->getName() == "C4")
    	ac->setCpuId(cpunb - 3);
    else
	ac->setCpuId(cpunb - 4);
  }
}

void Placement::manual4(const std::vector<Actor *> & actors) {

  for (auto ac : actors) {
    if (ac->getName() == "Src" || ac->getName() == "S"
        || ac->getName() == "A1" || ac->getName() == "B1"
	|| ac->getName() == "C1")
    	ac->setCpuId(cpunb - 1);
    else if (ac->getName() == "Snk" || ac->getName() == "J" 
	|| ac->getName() == "A2" || ac->getName() == "B3"
        || ac->getName() == "C4") 	
    	ac->setCpuId(cpunb - 2);
    else if (ac->getName() == "A3" || ac->getName() == "B4" 
		    || ac->getName() == "C5")
    	ac->setCpuId(cpunb - 3);
    else
	ac->setCpuId(cpunb - 4);
  }
}

