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

#include "timer.h"

using namespace std;
using namespace df;

Timer::Timer() {
}

void Timer::start() { 
	hrtstart = std::chrono::high_resolution_clock::now(); 
}

std::string Timer::end() { 
	hrtend = std::chrono::high_resolution_clock::now(); 
	return std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(hrtend - hrtstart).count()); 
}

std::string Timer::endUs() { 
	hrtend = std::chrono::high_resolution_clock::now(); 
	return std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(hrtend - hrtstart).count()); 
}

void Timer::startClock() { 
	tstart = clock(); 
}

std::string Timer::endClock() { 
	tend = clock();
	return std::to_string(double(tend - tstart)/CLOCKS_PER_SEC); 
}

unsigned long Timer::now() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Timer::randSleep(int s) {
  this_thread::sleep_for(chrono::milliseconds{rand()%s});
}

void Timer::sleep(int s) {
  this_thread::sleep_for(chrono::milliseconds{s});
}

