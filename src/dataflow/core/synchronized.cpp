/*
 *   libdataflow
 *   Copyright (C) 2018
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

#include "synchronized.h"

using namespace std;
using namespace df;

Synchronized::Synchronized(): consuming(0), producing(false), consumed(0), produced(false), total_consumer(0) {
} 

void Synchronized::addConsumer() {
  total_consumer++;
}

void Synchronized::consumerLock() {
  unique_lock<mutex> locker(mux);
  while(producing || !produced)
    pro_cond.wait(locker);
  
  consuming++;
  consumed++;
  
  if(consumed == total_consumer) {
    consumed = 0;
    produced = false;
  }
}

void Synchronized::consumerUnlock() {
  lock_guard<mutex> locker(mux);
  consuming--;
  con_cond.notify_one();
}

void Synchronized::producerLock() {
  unique_lock<mutex> locker(mux);
  while(consuming > 0 || produced)
    con_cond.wait(locker);
  
  produced = true;
  producing = true;
}

bool Synchronized::producerRTLock() {
  lock_guard<mutex> locker(mux);
  if(consuming > 0)
    return false;
  
  produced = true;
  producing = true;
  return true;
}

void Synchronized::producerUnlock() {
  lock_guard<mutex> locker(mux);
  producing = false;
  pro_cond.notify_all();
}
