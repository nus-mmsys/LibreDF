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

#include "core/SampleSynchronizer.h"

using namespace std;
using namespace tmf;

SampleSynchronizer::SampleSynchronizer(): con_num(0), prod(false), consumed(0), produced(false), total_consumer(0) {
} 

void SampleSynchronizer::addConsumer() {
  total_consumer++;
}

void SampleSynchronizer::consumerLock() {
  unique_lock<mutex> locker(mux);
  while(prod || !produced)
    pro_cond.wait(locker);
  
  con_num++;
  consumed++;
  
  if(consumed == total_consumer) {
    consumed = 0;
    produced = false;
  }
}

void SampleSynchronizer::consumerUnlock() {
  lock_guard<mutex> locker(mux);
  con_num--;
  con_cond.notify_one();
}

void SampleSynchronizer::producerLock() {
  unique_lock<mutex> locker(mux);
  while(con_num > 0 || produced)
    con_cond.wait(locker);
  
  produced = true;
  prod = true;
}

bool SampleSynchronizer::producerRTLock() {
  lock_guard<mutex> locker(mux);
  if(con_num > 0)
    return false;
  
  produced = true;
  prod = true;
  return true;
}

void SampleSynchronizer::producerUnlock() {
  lock_guard<mutex> locker(mux);
  prod = false;
  pro_cond.notify_all();
}
