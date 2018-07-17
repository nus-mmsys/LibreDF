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

#ifndef DF_H_
#define DF_H_

#include <string>
#include <iostream>
#include <map>
#include <cstdarg>
#include <sstream>

#include "actor_factory.h"
#include "actor.h"
#include "edge.h"
#include "server_socket.h"
#include "client_socket.h"

namespace df {
  
  /*!
   * \enum DataflowStatus
   * Status of a actor.
   */
  enum class DataflowStatus {
    NONE, /**<The dataflow has not started yet.*/
    STOPPED, /**< The dataflow has stopped. */
    READY,  /**< The dataflow is ready. */
    RUNNING, /**< The dataflow is currently running. */
    PAUSED, /**< The dataflow is currently paused. */
  };
  /*!
   * \class Dataflow
   * A dataflow, consisting of a number of interconnected actors.
   * Actors have a many-to-many relation, with directed edges. Cycles are not allowed.
   */
  class Dataflow {
  private:

    std::thread tdisc;
    std::mutex io_lock;
    std::string name; /**< The name of the dataflow. */
    DataflowStatus status; /**< The current status of the dataflow. */
    std::map<std::string, Actor *> actors; /**< The set of all actors in the dataflow. */
    std::map<std::string, Edge *> edges; /**< The set of all actors in the dataflow. */
    std::map<std::string, Actor *> remoteactors; /**< The set of all remote actors in the dataflow. */
    bool realtime, distributed;
    std::string dischost;
    int discport;
    Property prop;
    ServerSocket * srvsock;
    ClientSocket * clnsock;
    
    void discovery();

  public:

    using actor_iter = std::map<std::string, Actor *>::const_iterator;	
    using edge_iter = std::map<std::string, Edge *>::const_iterator;	

    actor_iter actorBegin();
    actor_iter actorEnd();
    int actorSize();
    
    edge_iter edgeBegin();
    edge_iter edgeEnd();
    int edgeSize();

    /*!
     * Dataflow constructor
     *
     * \param name The name of the dataflow.
     */
    Dataflow(const std::string& name);

    std::string getName();

    template<typename T>
    void setProp(const std::string& key, const T& val) {
	    prop.setProp(key,val);
    }

    template<typename T>
    void replaceProp(const std::string& key, const T& val) {
	    prop.replaceProp(key,val);
    }

    Edge * createEdge(const std::string& name, const std::string& src,
		      const std::string& snk);

    Actor * createActor(std::string const& s, const std::string& name);
    Actor * createRemoteActor(const std::string& name);

    void addActor(Actor * f);
    
    void addRemoteActor(Actor * f);
    
    void addActors(Actor * f, ...);
    
    /*!
     * Create an edge between two actors in the dataflow.
     * These actors should be in the dataflow.
     *
     * \param src The source actor for the edge.
     * \param snk The sink actor for the edge.
     */
    void connectActors(Actor * src, Actor * snk);
   
    /*!
     * Create an edge between two actors in the dataflow
     * with the corresponding rates.
     * These actors should be in the dataflow.
     *
     * \param src The source actor for the edge.
     * \param snk The sink actor for the edge.
     * \param p The production rate of the source actor for the edge.
     * \param c The consumption rate of the sink actor for the edge.
     */
    void connectActors(Actor * src, Actor * snk, int p, int c);

    /*!
     * Create an edge between two actors in the dataflow
     * with the corresponding rates.
     * These actors should be in the dataflow.
     *
     * \param src The source actor for the edge.
     * \param snk The sink actor for the edge.
     * \param edge The edge on which two actors will be connected.
     * \param p The production rate of the source actor.
     * \param c The consumption rate of the sink actor.
     */
    void connectActors(Actor * src, Actor * snk, std::string edge, int p, int c);

    void runDiscovery();
    void waitDiscovery();
    
    /*!
     * Initialize the dataflow.
     *
     */
    void init();
   
    void connect();

    /*!
     * Run one iteration of the dataflow.
     *
     */
    void run();
    
    /*!
     * Dataflow destructor
     */
    ~Dataflow();
  };
  
}
#endif /* DF_DATAFLOW_H_ */
