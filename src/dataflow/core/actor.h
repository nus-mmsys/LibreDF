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

#ifndef DF_ACTOR_H_
#define DF_ACTOR_H_

#include "attribute.h"
#include "port.h"
#include "input_port.h"
#include "output_port.h"
#include "event.h"
#include "bus.h"

#include <vector>
#include <map>
#include <thread>
#include <iostream>
#include <sys/stat.h>

namespace df {
  
  /*!
   * \enum void
   * Status of a actor.
   */
  enum class ActorStatus {
    OK, /**< Actor processed successfully. */
    ERROR, /**< An error occurred while processing. */
    EOS, /**< Actor is done generating more data. Used in data sources. */
  };

 
  /*!
   * \class Actor
   * Abstraction of a actor in a dataflow.
   * Every concrete actor inherits from actor and can be connected to multiple actors,
   * and receive various data from predecessor actors and send data to accessor actor.
   */
  
  class Actor : public EventObserver {
  private:

    	  
    std::thread tinit;
    std::thread trun;
    std::mutex * dataflowlock;

    Attribute attr; /**< A map containing the message keys and values transfered to actor from a dataflow */
    Bus * busref;
    
    bool realtime;
    struct stat stat_info; /**< It is to ckeck if the rdf_path exists */

  protected:

    std::string name; /**< The name of the actor */
    int stepno; /**< The number of step in which the actor is in */ 
    std::string home_path; /**< The path for home folder */    
    std::string df_path; /**< The path for actors to use */
    std::string dfout_path; /**< The path for actors to use as output */
   
    std::vector<Port*> inputPorts; /**< List of the input ports  */
    std::vector<Port*> outputPorts; /**< List of the output ports */

    clock_t tstart;
    clock_t tend;
    void start() { 
	    tstart = clock(); 
    }
    void end(std::string msg) { 
	    tend = clock();
	    log(msg+" exect = "+std::to_string(double(tend - tstart)/CLOCKS_PER_SEC)); 
    }

    ActorStatus status; 
    /*!
     * Actor constructor
     * \param name
     *   The name of the actor.
     */
    Actor(const std::string & name);
    
    void log(std::string msg);
    void sleep(int s); 
    
    /*!
     * Virtual function, to be implemented in the subclass actors.
     * Read data from input actor, process the data, and write the result to the output port.
     */
    virtual void init() {}
    virtual void run() = 0;
    virtual void runRT() {
      run();
    }
    
  public:
    
    void setRealTime(bool rt);
    
    /*!
     * Set a property of the actor.
     *
     * \param key
     *   The property name.
     * \param val
     *   The property value.
     */
    template<typename T>
    void setProp(const std::string & key, const T& val) {
      attr.setProp(key, val);
    }
    /*!
     * Get the value of a actor property.
     *
     * \param key
     *   The property name.
     */
    
    std::string getProp(const std::string & key) {
      return attr.getProp(key);
    } 
    
    /*!
     * Get the value (int) of a actor property.
     *
     * \param key
     *   The property name.
     */
    
    int getPropInt(const std::string & key) {
      return attr.getPropInt(key);
    } 

    /*!
     * Get the value (float) of a actor property.
     *
     * \param key
     *   The property name.
     */
    float getPropFloat(const std::string & key) {
      return attr.getPropFloat(key);
    }

    /*!
     * Get the value (float) of a actor property.
     *
     * \param key
     *   The property name.
     */
    bool propEmpty(const std::string & key) {
      return attr.propEmpty(key);
    }

    /*!
     * Connect this actor to another actor in the dataflow.
     * It is used by dataflow. User must use Dataflow::connectActors
     *
     * \param f
     *   The actor to connect to.
     */
    void connectActor(Actor * f);

    /*!
     * Connect this actor to another actor in the dataflow.
     * It is used by dataflow. User must use Dataflow::connectActors
     *
     * \param f
     *   The actor to connect to.
     * \param p
     *   The production rate of the actor
     * \param c
     *   The consumption rate of the actor f
     */
    void connectActor(Actor * f, int p, int c);
    
    /*!
     * Execute the init of this actor.
     * The actors are connected by a link list and each actor calls initActor of the next actor.
     *
     * \return The new status of the actor.
     */
    void initActor(); 
    
    /*!
     * Execute the processing of this actor.
     * The actors are connected by a link list and each actor calls executeActor of the next actor.
     *
     * \return The new status of the actor.
     */
    void runActor();
    
    void startInit();
    
    void startRun();
    
    void waitInit();
    
    void waitRun();
    
    void setPipeLock(std::mutex * mux);
    
    template <typename T>
    InputPort<T> * createInputPort(std::string name) {
      InputPort<T> * res = new InputPort<T>(name);
      this->inputPorts.push_back(res);
      return res;
    }
    
    template <typename T>
    OutputPort<T> * createOutputPort(std::string name) {
      OutputPort<T> * res = new OutputPort<T>(name);
      this->outputPorts.push_back(res);
      return res;
    }

    template<typename T>
    T * consume(InputPort<T> * port) {
      port->lock();
      T * data = port->get();
      if (port->getStatus() == TokenStatus::EOS)
        status = ActorStatus::EOS;
      return data; 
    }

    template<typename T>
    T * produce(OutputPort<T> * port) {
      port->lock();
      T * data = port->get();
      if (status == ActorStatus::EOS)
        port->setStatus(TokenStatus::EOS);
      return data;
    }

    template<typename T>
    void setEOS(OutputPort<T> * port) {
      port->lock();
      port->setStatus(TokenStatus::EOS);
      status = ActorStatus::EOS;
      port->unlock();
    }

    template<typename T>
    void release(InputPort<T> * port) {
      port->unlock();
    }
   
    template<typename T>
    void release(OutputPort<T> * port) {
      port->unlock();
    }

    void handleEvent(Event event);
    
    void sendEvent(Event event);
    
    void destroyPort(Port * port) {
      delete port;
    }
    void setBusRef(Bus * b) { busref = b; } 
    ActorStatus getStatus() { return status; }
    /*!
     * Destructor of the actor.
     */
    virtual ~Actor();
  };
  
}
#endif /* DF_ACTOR_H_ */
