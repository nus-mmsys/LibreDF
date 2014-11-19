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

#ifndef FITLER_H_
#define FITLER_H_

#include "core/Attribute.h"
#include "core/Message.h"
#include "core/Port.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <thread>

using namespace std;

//template <typename T> class Port<T>;

/*!
 * \enum FilterStatus
 * Status of a filter.
 */
enum FilterStatus {
  FILTER_SUCCESS, /**< Filter processed successfully. */
  FILTER_ERROR, /**< An error occurred while processing. */
  FILTER_FINISHED, /**< Filter is done generating more data. Used in data sources. */
};

/*!
 * \class Filter
 * Abstraction of a filter in a pipeline.
 * Every concrete filter inherits from filter and can be connected to multiple filters,
 * and receive various data from predecessor filters and send data to accessor filter.
 */

class Filter {
private:
  
  thread * t;
  mutex * io_lock;
  
  string name; /**< The name f the filter */
  Attribute attr; /**< A map containing the message keys and values transfered to filter from a pipeline */
  
  bool realtime;
  
protected:
  Message * inMsg; /**< Input message of the filter */
  Message * outMsg; /**< Output message of the filter */
  
  vector<Port*> inputPorts; /**< List of the input ports  */
  vector<Port*> outputPorts; /**< List of the output ports */
  
  /*!
   * Filter constructor
   * \param name
   *   The name of the filter.
   */
  Filter(const string & name);
  
  void log(std::string msg);
  void sleep(int s); 
  
  /*!
   * Virtual function, to be implemented in the subclass filters.
   * Read data from input filter, process the data, and write the result to the output port.
   */
  virtual FilterStatus init() {
    return FILTER_SUCCESS;
  }
  virtual FilterStatus run() = 0;
  virtual FilterStatus runRT() {
    return run();
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
  void setProp(const string & key, const T& val) {
    attr.setProp(key, val);
  }
  /*!
   * Get the value of a filter property.
   *
   * \param key
   *   The property name.
   */
  
  string&& getProp(const string & key) {
    return move(attr.getProp(key));
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
  FilterStatus initFilter(); 
  
  /*!
   * Execute the processing of this filter.
   * The filters are connected by a link list and each filter calls executeFilter of the next filter.
   *
   * \return The new status of the filter.
   */
  FilterStatus runFilter();
  
  /*!
   * Get the number of input ports.
   */
  //int inputPortNum();
  
  /*!
   * Get the number of output port.
   */
  int outputPortNum();
  
  void startInit();
  
  void startRun();
  
  void wait();
  
  void setIOLock(mutex * mux);
  
  /*!
   * Destructor of the filter.
   */
  virtual ~Filter();
};

#endif /* FITLER_H_ */
