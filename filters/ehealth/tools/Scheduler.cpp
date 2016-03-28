/*
 * 
 *  Tiny Multimedia Framework (tmf)
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This file is part of tmf.
 *
 *  tmf is free software; you can redistribute it and/or 
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  tmf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with tmf.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "filters/ehealth/tools/Scheduler.h"


Scheduler::Scheduler(int min, int max): minPeriod(min), maxPeriod(max), currentTime(0) {

}

void Scheduler::start() {
  startTime = std::chrono::high_resolution_clock::now();
}

void Scheduler::end() {
  endTime = std::chrono::high_resolution_clock::now();
  
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( endTime - startTime ).count();

  currentTime+=minPeriod;
  if (currentTime >=  maxPeriod)
    currentTime = 0;

  std::this_thread::sleep_for(std::chrono::microseconds(minPeriod*1000 - duration));
}

bool Scheduler::timeToExecute(int period) {
  return (currentTime % period == 0);
}

Scheduler::~Scheduler() {

}
