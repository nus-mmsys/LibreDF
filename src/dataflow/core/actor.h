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

#ifndef DF_ACTOR_H_
#define DF_ACTOR_H_

#include "property.h"
#include "port.h"
#include "input_port.h"
#include "output_port.h"

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
  
  class Actor  {
  private:

    	  
    std::thread tinit;
    std::thread trun;
    std::mutex * dataflowlock;

    Property prop; /**< A map containing the message keys and values transfered to actor from a dataflow */
    
    struct stat stat_info; /**< It is to ckeck if the rdf_path exists */

    std::chrono::high_resolution_clock::time_point hrtstart; 
    std::chrono::high_resolution_clock::time_point hrtend; 
    clock_t tstart;
    clock_t tend;

  protected:

    std::string name; /**< The name of the actor */
    int stepno; /**< The number of step in which the actor is in */ 
    std::string home_path; /**< The path for home folder */    
    std::string df_path; /**< The path for actors to use */
    std::string dfout_path; /**< The path for actors to use as output */
    
    std::map<std::string, Port*>inputPorts; /**< Map of input ports referenced by their name  */
    std::map<std::string, Port*> outputPorts; /**< Map of output ports referenced by their name */

    bool distributed, realtime;
    ActorStatus status; 
   
    /*!
     * Timing measurement
     * hstart, hend : high_resolution_clock
     * start, end   : clock_t
     */ 
    void hstart();
    void hend(std::string msg); 
    void start();
    void end(std::string msg); 

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
    virtual void runDist() {
      run();
    }
   
  public:

    std::string getName();

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
      prop.setProp(key, val);
    }
    /*!
     * Get the value of a actor property.
     *
     * \param key
     *   The property name.
     */
    
    std::string getProp(const std::string & key) {
      return prop.getProp(key);
    } 

    /*!
     * Get the value (bool) of a actor property.
     *
     * \param key
     *   The property name.
     */
    int getPropBool(const std::string & key) {
      return prop.getPropBool(key);
    } 
 
    /*!
     * Get the value (int) of a actor property.
     *
     * \param key
     *   The property name.
     */
    
    int getPropInt(const std::string & key) {
      return prop.getPropInt(key);
    } 

    /*!
     * Get the value (float) of a actor property.
     *
     * \param key
     *   The property name.
     */
    float getPropFloat(const std::string & key) {
      return prop.getPropFloat(key);
    }

    /*!
     * Get the value (float) of a actor property.
     *
     * \param key
     *   The property name.
     */
    bool propEmpty(const std::string & key) {
      return prop.propEmpty(key);
    }

    /*!
     * Get the input port associated to an edge
     *
     * \param edgename
     *   The name of the edge.
     *
     * \return 
     * 	 The name of the associated input port.
     */
    std::string edge2InputPort(std::string edgename);

    /*!
     * Get the output port associated to an edge
     *
     * \param edgename
     *   The name of the edge.
     * \param index
     *   If the associated port name is indexed, 
     *   then the index is filled. For example if 
     *   the associated port is "output[3]", then the
     *   returned value is "output" and index is set 
     *   to 3. 
     *
     * \return 
     * 	 The name of the associated output port.
     * 
     */
    std::string edge2OutputPort(std::string edgename, int & index);


    /*!
     * Connect the port with a specified port name of this actor 
     * to another actor listening on a port number on a host
     * It is used by dataflow. User must use Dataflow::connectActors
     *
     * \param portname
     *   The port name of this to connect.
     * \param host
     *   The host of another actor to connect to.
     * \param portnb
     *   The port number of another actor to connect to.
     *
     */
    int connectActor(std::string edge, std::string host, int portnb);

    /*!
     * Connect this actor to another actor in the dataflow.
     * It is used by dataflow. User must use Dataflow::connectActors
     *
     * \param snk
     *   The actor to connect to.
     */
    int connectActor(Actor * snk);

    /*!
     * Connect this actor to another actor in the dataflow.
     * It is used by dataflow. User must use Dataflow::connectActors
     *
     * \param snk
     *   The actor to connect to.
     * \param p
     *   The production rate of this actor.
     * \param c
     *   The consumption rate of snk actor.
     */
    int connectActor(Actor * snk, int p, int c);

    /*!
     * Connect this actor to another actor in the dataflow.
     * It is used by dataflow. User must use Dataflow::connectActors
     *
     * \param snk
     *   The actor to connect to.
     * \param edge
     * 	 The edge on which acotr will be connected.
     * \param p
     *   The production rate of the actor.
     * \param c
     *   The consumption rate of snk actor.
     */
    int connectActor(Actor * snk, std::string edge, int p, int c);

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
      this->inputPorts.insert(std::make_pair(name, res));
      return res;
    }

    template <typename T>
    OutputPort<T> * createOutputPort(std::string name) {
      OutputPort<T> * res = new OutputPort<T>(name);
      this->outputPorts.insert(std::make_pair(name, res));
      return res;
    }

    template<typename T>
    T * consume(InputPort<T> * port) {
      if (distributed) {
	T * res = port->recv();
	if (res == nullptr)
		log(getName()+" did recieve on port "+port->getName());
      	return res;
      }
      else {
	port->lock();
        if (port->getStatus() == TokenStatus::EOS)
          status = ActorStatus::EOS;
        return port->get();
      }	
    }

    template<typename T>
    T * produce(OutputPort<T> * port) {
      if (distributed) {
	return port->getdata();
      }
      else {
        port->lock();
        if (status == ActorStatus::EOS)
          port->setStatus(TokenStatus::EOS);
        return port->get();
      }
    }

    template<typename T>
    void setEos(OutputPort<T> * port) {
      port->setStatus(TokenStatus::EOS);
      status = ActorStatus::EOS;
    }
    
    template<typename T>
    void release(InputPort<T> * port) {
      if (!distributed)
	    port->unlock();
    }
   
    template<typename T>
    void release(OutputPort<T> * port) {
      if (distributed)
	    port->send();
      else
	    port->unlock();
    }

    void listen(Port * port); 

    void destroyPort(Port * port) {
      delete port;
    }
    ActorStatus getStatus() { return status; }
    /*!
     * Destructor of the actor.
     */
    virtual ~Actor();
  };
  
}
#endif /* DF_ACTOR_H_ */
