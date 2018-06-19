/*
 *   DF
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

#include "token_synchronizer.h"

using namespace std;
using namespace df;

TokenSynchronizer::TokenSynchronizer(): con_num(0), prod(false), consumed(0), produced(false), total_consumer(0) {
} 

void TokenSynchronizer::addConsumer() {
  total_consumer++;
}

void TokenSynchronizer::consumerLock() {
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

void TokenSynchronizer::consumerUnlock() {
  lock_guard<mutex> locker(mux);
  con_num--;
  con_cond.notify_one();
}

void TokenSynchronizer::producerLock() {
  unique_lock<mutex> locker(mux);
  while(con_num > 0 || produced)
    con_cond.wait(locker);
  
  produced = true;
  prod = true;
}

bool TokenSynchronizer::producerRTLock() {
  lock_guard<mutex> locker(mux);
  if(con_num > 0)
    return false;
  
  produced = true;
  prod = true;
  return true;
}

void TokenSynchronizer::producerUnlock() {
  lock_guard<mutex> locker(mux);
  prod = false;
  pro_cond.notify_all();
}
