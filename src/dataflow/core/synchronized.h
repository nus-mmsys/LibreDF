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

#ifndef DF_SYNCHRONIZED_H_
#define DF_SYNCHRONIZED_H_

#include <mutex>
#include <condition_variable>

namespace df {
  
  class Synchronized {
    
    std::mutex mux;
    
    std::condition_variable con_cond;
    std::condition_variable pro_cond;
    
    unsigned int consuming;
    unsigned int consumed;
    unsigned int total_consumer;
    
    bool producing;
    bool produced;
    
  public:
    
    Synchronized();
    
    void addConsumer();
   
    void clearConsumer();

    /*!
     * Acquiring and releasing consumer lock
     *
     * the consumer can acqure the lock only if 
     * 		(1) no producer is in critical section (producing=false)
     * 		(2) a producer has produced (produced=true)
     *
     * once the consumer acquires the lock, it
     * 		(1) increases the number of consumers 
     * 		    in the critical section (con_num++)
     * 		(2) increases the number of consumers 
     * 		    who have consumed (consumed++)
     * 		(3) if he is the last consumer consuming,
     * 		    it puts the number of consumers to zero (consumed=0), 
     * 		    and flags the section as ready for 
     * 		    the producer (produced=false)
     * 
     * once the consumer exits the critical section, it
     * 		(1) reduces the number of consumers 
     * 		    in the critical section (con_num--)
     */
    void consumerLock();
    void consumerUnlock();
    
    /*!
     * Acquiring and releasing producer lock
     *
     * the producer can acqure the lock only if 
     * 		(1) no consumer is in critical section (con_num=0)
     * 		(2) the section is flaged as ready to producer (produced=false)
     * 
     * once the producer acquires the lock, it
     * 		(1) flags the section as ready to consumer (produced=true)
     * 		(2) sets a flag to notify the presence of producer 
     * 		    in the critical section (producing=true)
     * 
     * once the producer exits the critical section, it
     * 		(1) sets the flag to notify the absence of producer
     * 		    in the critical section (producing=false)
     */
    void producerLock();
    void producerUnlock();

    /*!
     * Acquiring real-time producer lock
     *
     * the producer can acqure the lock only if 
     * 		(1) no consumer is in critical section (con_num=0)
     * 		    if the consumer is in the critical section it returns
     * 		    false, which gives extra information to the user
     * 		    of the critical section.
     * 		    In a circular buffer, the user of the 
     * 		    critical section can decide to advance 
     * 		    in the buffer and re-write old values.
     * 
     * once the producer acquires the lock, it
     * 		(1) flags the section as ready to consumer (produced=true)
     * 		(2) sets a flag to notify the presence of producer 
     * 		    in the critical section (producing=true)
     */
    bool producerRTLock();
    
  };
  
}
#endif /* DF_SYNCHRONIZED_H_ */
