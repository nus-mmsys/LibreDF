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

#ifndef FITLER_H_
#define FITLER_H_

#include "core/Attribute.h"
#include "core/Port.h"
#include "core/InputPort.h"
#include "core/OutputPort.h"
#include "core/Event.h"
#include "core/Bus.h"

#include <vector>
#include <map>
#include <thread>
#include <iostream>

namespace tmf {
  
  /*!
   * \enum void
   * Status of a filter.
   */
  enum class FilterStatus {
    OK, /**< Filter processed successfully. */
    ERROR, /**< An error occurred while processing. */
    EOS, /**< Filter is done generating more data. Used in data sources. */
  };
  
  /*!
   * \class Filter
   * Abstraction of a filter in a pipeline.
   * Every concrete filter inherits from filter and can be connected to multiple filters,
   * and receive various data from predecessor filters and send data to accessor filter.
   */
  
  class Filter : public EventObserver {
  private:
    
    std::thread tinit;
    std::thread trun;
    std::mutex * pipelock;
    
    std::string name; /**< The name f the filter */
    Attribute attr; /**< A map containing the message keys and values transfered to filter from a pipeline */
    Bus * busref;
    
    bool realtime;
    
  protected:
    
    std::vector<Port*> inputPorts; /**< List of the input ports  */
    std::vector<Port*> outputPorts; /**< List of the output ports */
    
    FilterStatus status; 
    /*!
     * Filter constructor
     * \param name
     *   The name of the filter.
     */
    Filter(const std::string & name);
    
    void log(std::string msg);
    void sleep(int s); 
    
    /*!
     * Virtual function, to be implemented in the subclass filters.
     * Read data from input filter, process the data, and write the result to the output port.
     */
    virtual void init() {}
    virtual void run() = 0;
    virtual void runRT() {
      run();
    }
    
  public:
    
    void setRealTime(bool rt);
    
    /*!
     * Set a property of the filter.
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
     * Get the value of a filter property.
     *
     * \param key
     *   The property name.
     */
    
    std::string getProp(const std::string & key) {
      return attr.getProp(key);
    } 
    
    /*!
     * Connect this filter to another filter in the pipeline.
     * It is used by pipeline. User must use Pipeline::connectFilters
     *
     * \param f
     *   The filter to connect to.
     */
    void connectFilter(Filter * f);
    
    /*!
     * Execute the init of this filter.
     * The filters are connected by a link list and each filter calls initFilter of the next filter.
     *
     * \return The new status of the filter.
     */
    void initFilter(); 
    
    /*!
     * Execute the processing of this filter.
     * The filters are connected by a link list and each filter calls executeFilter of the next filter.
     *
     * \return The new status of the filter.
     */
    void runFilter();
    
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
    
    void handleEvent(Event event);
    
    void sendEvent(Event event);
    
    void destroyPort(Port * port) {
      delete port;
    }
    void setBusRef(Bus * b) { busref = b; } 
    FilterStatus getStatus() { return status; }
    /*!
     * Destructor of the filter.
     */
    virtual ~Filter();
  };
  
}
#endif /* FITLER_H_ */
