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

#ifndef DF_SYNCHRONIZED_H_
#define DF_SYNCHRONIZED_H_

#include <mutex>
#include <condition_variable>

namespace df {
  
  class Synchronized {
    
    std::mutex mux;
    
    std::condition_variable con_cond;
    unsigned int con_num;
    
    std::condition_variable pro_cond;
    bool prod;
    
    unsigned int consumed;
    bool produced;
    unsigned int total_consumer;
    
  public:
    
    Synchronized();
    
    void addConsumer();
    
    void consumerLock();
    
    void consumerUnlock();
    
    void producerLock();
    
    bool producerRTLock();
    
    void producerUnlock();
    
  };
  
}
#endif /* DF_SYNCHRONIZED_H_ */
