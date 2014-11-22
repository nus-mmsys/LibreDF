/*
 *
 *  Tiny Multimedia Framework
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef SAMPLESYNCHRONIZER_H_ 
#define SAMPLESYNCHRONIZER_H_

#include <mutex>
#include <condition_variable>

using namespace std;

class SampleSynchronizer {
 
  mutex mux;
  
  condition_variable con_cond;
  unsigned int con_num;
  
  condition_variable pro_cond;
  bool prod;
  
  unsigned int consumed;
  bool produced;
  unsigned int total_consumer;
 
public:
 
  SampleSynchronizer(): con_num(0), prod(false), consumed(0), produced(false), total_consumer(0) {
  } 

  void addConsumer() {
    total_consumer++;
  }
  
  void consumerLock() {
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
    
  void consumerUnlock() {
    lock_guard<mutex> locker(mux);
    con_num--;
    con_cond.notify_one();
  }
  
  void producerLock() {
    unique_lock<mutex> locker(mux);
    while(con_num > 0 || produced)
      con_cond.wait(locker);
    
    produced = true;
    prod = true;
  }
  
  bool producerRTLock() {
    lock_guard<mutex> locker(mux);
    if(con_num > 0)
      return false;
    
    produced = true;
    prod = true;
    return true;
  }
  
  void producerUnlock() {
    lock_guard<mutex> locker(mux);
    prod = false;
    pro_cond.notify_all();
  }

};

#endif /* SAMPLESYNCHRONIZER_H_ */
