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

#ifndef DF_PORT_VECTOR_H_
#define DF_PORT_VECTOR_H_

#include "port.h"

namespace df {
  
  /*!
   * \class Port
   * Abstraction of a port in a actor.
   * A port can be either input port of output port.
   */
  
  class PortVector : public Port {

  public:
    
    /*!
     * PortiVector constructor
     *
     * \param name The name of the actor.
     */
    PortVector(std::string name);
   
    /*!
     * Port descructor
     *
     */
    virtual ~PortVector() {}
  };
  
}
#endif /* DF_PORT_VECTOR_H_ */
