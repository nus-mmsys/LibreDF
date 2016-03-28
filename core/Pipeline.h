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

#ifndef PIPELINE_H_
#define PIPELINE_H_

#include <iostream>
#include <set>
#include <cstdarg>

#include "core/Filter.h"
#include "core/Bus.h"

namespace tmf {
  
  /*!
   * \enum PipelineStatus
   * Status of a filter.
   */
  enum class PipelineStatus {
    STOPPED, /**< The pipeline has stopped. */
    READY,  /**< The pipeline is ready. */
    RUNNING, /**< The pipeline is currently running. */
    PAUSED, /**< The pipeline is currently paused. */
  };
  /*!
   * \class Pipeline
   * A pipeline, consisting of a number of interconnected filters.
   * Filters have a many-to-many relation, with directed pipes. Cycles are not allowed.
   */
  class Pipeline {
    std::mutex io_lock;
    std::string name; /**< The name of the pipeline. */
    PipelineStatus status; /**< The current status of the pipeline. */
    Bus bus;
    std::set<Filter *> filters; /**< The set of all filters in the pipeline. */
    bool realtime;
  public:
    /*!
     * Pipeline constructor
     *
     * \param name The name of the pipeline.
     */
    Pipeline(const std::string& name);
    
    void setRealTime(bool);
    
    void addFilter(Filter * f);
    
    void addFilters(Filter * f, ...);
    
    /*!
     * Create a pipe between two filters in the pipeline.
     * These filters should be in the pipeline.
     *
     * \param fi The source filter for the pipe.
     * \param fo The target filter for the pipe.
     */
    void connectFilters(Filter * fi, Filter * fo);
    
    /*!
     * Initialize the pipeline.
     *
     */
    void init();
    
    /*!
     * Run one iteration of the pipeline.
     *
     */
    void run();
    
    /*!
     * Pipeline destructor
     */
    ~Pipeline();
  };
  
}
#endif /* PIPELINE_H_ */
