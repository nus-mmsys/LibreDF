/*
 *   LibreDF
 *   Copyright (C) A. Shafiei
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

#include "affinity.h"
#include "property.h"
#include "port.h"
#include "input_port.h"
#include "output_port.h"
#include "input_port_vector.h"
#include "output_port_vector.h"
#include "timer.h"
#include "file_system.h"

#include <vector>
#include <queue>
#include <map>
#include <thread>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

namespace df {
 
  /*!
   * \class Actor
   * Abstraction of an actor in a dataflow.
   * Every concrete actor inherits from actor and can be connected to multiple actors.
   * An actor reads from an input ports (waits if the buffers is empty), and writes to its output ports (waits if the buffers are full).
   */
  
  class Actor  {

  protected:

    std::string name; /**< The name of the actor */
    std::string type; /**< The type of the actor */
    Status status; 
    unsigned long creation_time;
    unsigned long stime;
    std::map<std::string, IPort*> inputPorts; /**< Map of input ports referenced by their name  */
    std::map<std::string, OPort*> outputPorts; /**< Map of output ports referenced by their name */

    int stepno; /**< The number of firing (step) in which the actor is in */
    int solution; /**< The actor's solution */ 
    int iterno; /**< The number of iteration in which the actor is in */
    int fireno; /**< The number of firing in one iteration */
    
    Timer timer;
    FileSystem fsys; /**< The actors can retrieve the path to use */

    /*!
     * Actor constructor
     * \param name
     *   The name of the actor.
     */
    Actor(const std::string & name);
   
    /*!
     * Virtual function, to be implemented in the subclass actors.
     * Read data from input actor, process the data, and write the result to the output port.
     */
    virtual void init() = 0;
    virtual void reinit() {
	    init();
    }
    virtual void start() {
    }
    virtual void run() = 0;
    virtual void runRT() {
      run();
    }

    void log(std::string msg);
    void execlog();
 
    int average_period;
    int instance_period;
    unsigned long latency;
  
    unsigned long start_exec, end_exec, exec_dur;
  private:

    Property prop; /**< A map containing the message keys and values transfered to actor from a dataflow */
    
    int cpuid;
    bool logging;
    bool scheduling;  
    bool distributed;
    bool realtime;

    std::ofstream logfile;

    std::thread tinit;
    std::thread treinit;
    std::thread trun;
    std::thread tresumetill;
    std::thread tpause;

    int iter_max;
    bool paused;
    std::condition_variable pause_cond;
    std::condition_variable sol_cond;
    std::mutex pause_mux;
    std::mutex sol_mux;
    std::mutex runend_mux;

    std::mutex * iolock;
    std::mutex status_mux;
    std::mutex time_mux;

    unsigned long start_iter, end_iter, start_firing, end_firing;
  public:

    std::string getName();

    std::string getType();
    
    void setType(std::string t);

    void setSolution(int sol);

    int getSolution();

    void setCpuId(int cpu);

    int getCpuId();
    
    Status getStatus() {
	Status res;
	status_mux.lock();
	res = status;
	status_mux.unlock();
	return res;
    }

    void setStatus(Status st) {
	status_mux.lock();
	status = st;
	status_mux.unlock();
    }

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
     * Set all properties of the actor.
     * If a property already exists, it replace its value.
     *
     * \param p
     *   The list of all property names and values.
     *
     * \return
     *   Returns true if a new entry is added or an 
     *   old value is replaced by a new one.
     */
    bool setProps(std::map<std::string, std::string> p);
    
    /*!
     * Replace a property of the actor.
     *
     * \param key
     *   The property name.
     * \param val
     *   The property value.
     */
    template<typename T>
    void replaceProp(const std::string & key, const T& val) {
      prop.replaceProp(key, val);
    }

    /*!
     * Get the list of all actor property.
     *
     */ 
    std::map<std::string, std::string> getProps() {
      return prop.getProps();
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

    std::string getSingleInputPort();
    std::string getSingleOutputPort();

    int setInputPortRate(const std::string& portname, int rate);
    int setOutputPortRate(const std::string& portname, int rate);

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
    int connectActor(std::string outp, std::string host, int portnb);

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
     * \param outp
     * 	 The output port on which actor will be connected.
     * \param inp
     *   The input port on which the sink actor will be connected.
     * \param p
     *   The production rate of the actor.
     * \param c
     *   The consumption rate of snk actor.
     */
    int connectActor(Actor * snk, std::string outp, std::string inp,
		   int & outpidx, int & inpidx, int p, int c);

    /*!
     * Disconnect this actor from another actor in the dataflow.
     *
     * \param snk
     *   The actor to disconnect from.
     * \param outp
     * 	 The output port on which actor is connected.
     * \param inp
     *   The input port on which the sink actor is connected.
     */
    int disconnectActor(Actor * snk, std::string outp, std::string inp, int outpidx, int inpidx);
    
    /*!
     * Execute the init of this actor.
     * The actors are connected by a link list and each actor calls initActor of the next actor.
     *
     * \return The new status of the actor.
     */
    void initActor(); 
     
    /*!
     * Execute the reinit of this actor.
     * (Used in reinitialization)
     *
     */
    void reInitActor(); 

    /*!
     * Execute the processing of this actor.
     * The actors are connected by a link list and each actor calls executeActor of the next actor.
     *
     * \return The new status of the actor.
     */
    void runActor();
    
    void runIter();
    
    void assignTime();
    void setTime(unsigned long);
    unsigned long getTime();
    unsigned long getStopWatch();

    void startInit();
    
    void startReInit();
    
    void startRun();
    
    void startResumeTill(int iter);
    
    void startPause();

    void waitPause();

    void waitInit();
    
    void waitReInit();
    
    void waitRun();
    
    void waitResumeTill();

    void setIOLock(std::mutex * mux);
    
    template <typename T>
    InputPort<T> * createInputPort(std::string name) {
      if (name.find(".") != std::string::npos) {
	      std::cerr << "port name " << name << " is not valid.\n";
	      return nullptr;
      } 
      InputPort<T> * res = new InputPort<T>(name);
      this->inputPorts.insert(std::make_pair(name, res));
      return res;
    }

    template <typename T>
    InputPortVector<T> * createInputPortVector(std::string name) {
      if (name.find(".") != std::string::npos) {
	      std::cerr << "port name " << name << " is not valid.\n";
	      return nullptr;
      } 
      InputPortVector<T> * res = new InputPortVector<T>(name);
      this->inputPorts.insert(std::make_pair(name, res));
      return res;
    }

    template <typename T>
    OutputPort<T> * createOutputPort(std::string name) {
      if (name.find(".") != std::string::npos) {
	      std::cerr << "port name " << name << " is not valid.\n";
	      return nullptr;
      } 
      OutputPort<T> * res = new OutputPort<T>(name);
      this->outputPorts.insert(std::make_pair(name, res));
      return res;
    }

    template <typename T>
    OutputPortVector<T> * createOutputPortVector(std::string name) {
      if (name.find(".") != std::string::npos) {
	      std::cerr << "port name " << name << " is not valid.\n";
	      return nullptr;
      } 
      OutputPortVector<T> * res = new OutputPortVector<T>(name);
      this->outputPorts.insert(std::make_pair(name, res));
      return res;
    }

    int rate(Port * port) {
	    return port->getRate();
    }

    template<typename T>
    T * consume(InputPort<T> * port) {
      T * res = nullptr;
      if (distributed) {
	res = port->recv();
	if (res == nullptr)
		log("cannot recieve on port "+port->getName());
	setStatus(res->getStatus());
      }
      else {
	port->lock();
	res = port->get();
	setTime(port->getTime());
	setStatus(port->getStatus());
      }
      return res;      
    }

    template<typename T>
    std::vector<T *> consumeMR(InputPort<T> * port) {
      std::vector<T *> res;
      if (distributed) {
	res = port->recvMR();
	if (res.empty())
		log("cannot recieve on port "+port->getName());
	setStatus(res[res.size()-1]->getStatus());
      }
      else {
	port->lockMR();
	res = port->getMR();
	setTime(port->getTime());
	setStatus(port->getStatusMR());
      }
      return res;      
    }
    
    template<typename T>
    std::vector<T *> consume(InputPortVector<T> * port) {
      std::vector<T *> res;
      T * token = nullptr;  
      for (int i=0; i < port->arity(); i++) {
        if (distributed) {
	  token = port->at(i)->recv();
	  if (token == nullptr)
	  	  log("cannot recieve on port "+port->at(i)->getName());
	  setStatus(token->getStatus());
        } else {
	  port->at(i)->lock();
          token = port->at(i)->get();
	  setTime(port->at(i)->getTime());
	  setStatus(port->at(i)->getStatus());
        }
	res.push_back(token);
      }
      return res;
    }

    template<typename T>
    T * produce(OutputPort<T> * port) {
      T * res;
      if (distributed) {
	res = port->getSocketData();
	res->setStatus(getStatus());
      }
      else {
        port->lock();
        res = port->get();
	port->setTime(getTime());
        port->setStatus(getStatus());
      }
      return res;
    }
    
    template<typename T>
    std::vector<T *> produceMR(OutputPort<T> * port) {
      std::vector<T *> res;
      if (distributed) {
	res = port->getSocketDataMR();
	res[res.size()-1]->setStatus(getStatus());
      }
      else {
        port->lockMR();
        res = port->getMR();
	port->setTime(getTime());
        port->setStatusMR(getStatus());
      }
      return res;
    }

    template<typename T>
    std::vector<T *> produce(OutputPortVector<T> * port) {
	std::vector<T *> res;
	T * token = nullptr;
	for (int i=0; i<port->arity(); i++) {
	    if (distributed) {
		token = port->at(i)->getSocketData();
		token->setStatus(getStatus());
	    } else {
        	port->at(i)->lock();
        	token = port->at(i)->get();
		port->at(i)->setTime(getTime());
        	port->at(i)->setStatus(getStatus());
      	    }
	    res.push_back(token);
    	}
	return res;
    }

    template<typename T>
    void setEos(OutputPort<T> * port) {
      setStatus(EOS);
      if (distributed)
	      port->setSocketStatus(getStatus());
      else
	      port->setStatus(getStatus());
    }
 
    template<typename T>
    void setEosMR(OutputPort<T> * port) {
      setStatus(EOS);
      if (distributed)
	      port->setSocketStatusMR(getStatus());
      else
	      port->setStatusMR(getStatus());
    }
   
    template<typename T>
    void setEos(OutputPortVector<T> * port) {
      setStatus(EOS);
      for (int i=0; i<port->arity(); i++) {
      	if (distributed)
	      port->at(i)->setSocketStatus(getStatus());
      	else
	      port->at(i)->setStatus(getStatus());
      }
    }

    template<typename T>
    void release(InputPort<T> * port) {
      if (!distributed)
	    port->unlock();
    }
 
    template<typename T>
    void releaseMR(InputPort<T> * port) {
      if (!distributed) {
	    setStatus(port->getStatusMR());
	    port->unlockMR();
      }
    }

    template<typename T>
    void release(InputPortVector<T> * port) {
      for (int i=0; i<port->arity(); i++) {
      	if (!distributed)
		    port->at(i)->unlock();
      }
    }
  
    template<typename T>
    void release(OutputPort<T> * port) {
      if (distributed)
	    port->send();
      else
	    port->unlock();
    }
 
    template<typename T>
    void releaseMR(OutputPort<T> * port) {
      if (distributed)
	    port->sendMR();
      else {
            port->setStatusMR(getStatus());
	    port->unlockMR();
      }
    }

    template<typename T>
    void release(OutputPortVector<T> * port) {
      
      for (int i=0; i<port->arity(); i++) {
      	if (distributed)
	    port->at(i)->send();
      	else
	    port->at(i)->unlock();
      }
    }

    void listen(IPort * port); 

    void destroyPort(Port * port) {
      delete port;
    }

    void setPeriod(int p);
    int getPeriod();
    unsigned long getLatency();
    int getOutPortOcc(std::string port, int idx);
    int getInPortOcc(std::string port, int idx);
    bool isSource();
    bool isEnv();
    bool isNonSource();
    bool isSink();

    /*!
     * Pause the actor execution.
     */
    int pause();

    void pauseAfter(int imax);
    bool isPaused();
    
    /*!
     * Resume the actor execution.
     */
    void resume();

    /*!
     * Set the iteration number.
     */
    void setIteration(int iter);

    /*!
     * Resume until an iteration number.
     */
    int resumeTill(int iter);
    
    void terminate();

    /*!
     * Destructor of the actor.
     */
    virtual ~Actor();
  };
  
}
#endif /* DF_ACTOR_H_ */
