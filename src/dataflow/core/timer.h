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

#ifndef DF_TIMER_H_
#define DF_TIMER_H_

#include <chrono>
#include <thread>
#include <string>

namespace df {
	
  class Timer {

    private:

      std::chrono::high_resolution_clock::time_point hrtstart; 
      std::chrono::high_resolution_clock::time_point hrtend; 
      clock_t tstart;
      clock_t tend;
   
    public:
      Timer();

      /*!
       * Timing measurement
       * start, end : use high_resolution_clock
       * startClock, endClock : use clock_t
       */

      void start();
      std::string end(); 
      std::string endUs(); 
      void startClock();
      std::string endClock();
      unsigned long now();
      void sleep(int s); 
      void randSleep(int s); 
  };

}

#endif /* DF_TIMER_H_ */
