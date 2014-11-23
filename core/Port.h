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

#ifndef PORT_H_
#define PORT_H_

#include "core/Attribute.h"
#include "core/MediaBuffer.h"
#include "core/PortCaps.h"

#include <typeinfo>
#include <thread>
#include <vector>
#include <map>
#include <string>

using namespace std;
/*!
 * \class Port
 * Abstraction of a port in a filter.
 * A port can be either input port of output port.
 */

class Port {
  
private:
  string name; /**< The name of the port */
  int linked; /**< The number of port connected to this port */
  
protected:
  
  MediaBuffer<Attribute> * attrbuf;
  int attrindex;
  PortCaps portCaps;
  
public:
  
  /*!
   * Port constructor
   *
   * \param name The name of the filter.
   */
  Port(string name);
  
  /*!
   * Get the name of the port
   *
   * \return the name of the port.
   */
  string getName();
  
  const PortCaps & getPortCaps() const;
  /*!
   * Get the number of the ports connected to this port
   *
   * \return the number of the port.
   */
  int getLinked();
  
  /*!
   * Increase the number of the ports linked to the port.
   * (The filter uses this function when it connects two filters)
   *
   */
  void increaseLinked();
  
  Attribute * getAttr();
  
  virtual void connectPort(Port* n) {}
  
  /*!
   * Port descructor
   *
   */
  virtual ~Port() {}
};

#endif /* PORT_H_ */