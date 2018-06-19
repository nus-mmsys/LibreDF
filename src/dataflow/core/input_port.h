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

#ifndef DF_INPUTPORT_H
#define DF_INPUTPORT_H

#include "port.h"

namespace df {
  
  /*!
   * \class InputPort
   *
   * InputPort class is a subclass of the Port class.
   * It is a class template and the type of the buffer of the port is a template.
   *
   */
  
  template <typename T>
  class InputPort: public Port {
    
  private:
    
    Buffer<T> * buf;
    int index;
    
  public:
    
    /*!
     * InputPort constructor
     *
     * \param name The name of the port
     *
     */
    InputPort<T>(std::string name) : Port(name), buf(nullptr), index(0) {
      portCaps.addCaps("template", std::string(typeid(T).name()));
    }
    
    void setBuffer(Buffer<T> * b) {
      buf = b;
      buf->addConsumer();
    }
    
    void setAttrBuffer(Buffer<Attribute> * attrb) {
      attrbuf = attrb;
      attrbuf->addConsumer();
    }
    
    void lockAttr() {
      attrbuf->at(attrindex)->consumerLock();
    }
    
    void unlockAttr() {
      attrbuf->at(attrindex)->consumerUnlock();
      attrindex = (attrindex+1) % attrbuf->getSize();
    }
    
    void lock() {
      buf->at(index)->consumerLock();
    }
    
    void unlock() {
      buf->at(index)->consumerUnlock();
      index = (index+1) % buf->getSize();
    }
    
    T * get() {
      return buf->at(index)->get();
    }
    
    void setStatus(TokenStatus st) {
      buf->at(index)->setStatus(st);
    }
    TokenStatus getStatus() {
      return buf->at(index)->getStatus();
    }  
    /*!
     * InputPort destructor
     *
     */
    virtual ~InputPort() {
    }
    
  };
  
}
#endif // DF_INPUTPORT_H
