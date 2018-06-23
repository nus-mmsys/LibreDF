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

#ifndef DF_DATAFLOW_H_
#define DF_DATAFLOW_H_

#include <iostream>
#include <set>
#include <cstdarg>

#include "actor.h"

namespace df {
  
  /*!
   * \enum DataflowStatus
   * Status of a actor.
   */
  enum class DataflowStatus {
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
    std::mutex io_lock;
    std::string name; /**< The name of the dataflow. */
    DataflowStatus status; /**< The current status of the dataflow. */
    std::set<Actor *> actors; /**< The set of all actors in the dataflow. */
    bool realtime;
  public:
    /*!
     * Dataflow constructor
     *
     * \param name The name of the dataflow.
     */
    Dataflow(const std::string& name);
    
    void setRealTime(bool);
    
    void addActor(Actor * f);
    
    void addActors(Actor * f, ...);
    
    /*!
     * Create an edge between two actors in the dataflow.
     * These actors should be in the dataflow.
     *
     * \param fi The source actor for the edge.
     * \param fo The target actor for the edge.
     */
    void connectActors(Actor * fi, Actor * fo);
   
    /*!
     * Create an edge between two actors in the dataflow
     * with the corresponding rates.
     * These actors should be in the dataflow.
     *
     * \param in The source actor for the edge.
     * \param out The target actor for the edge.
     * \param prate The production rate of the source actor for the edge.
     * \param crate The consumption rate of the sink actor for the edge.
     */
    void connectActors(Actor * in, Actor * out, int p, int c);

    /*!
     * Initialize the dataflow.
     *
     */
    void init();
    
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
