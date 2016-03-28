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

#ifndef INPUTPORT_H
#define INPUTPORT_H

#include "core/Port.h"

namespace tmf {
  
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
    
    MediaBuffer<T> * buf;
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
    
    void setBuffer(MediaBuffer<T> * b) {
      buf = b;
      buf->addConsumer();
    }
    
    void setAttrBuffer(MediaBuffer<Attribute> * attrb) {
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
    
    void setStatus(SampleStatus st) {
      buf->at(index)->setStatus(st);
    }
    SampleStatus getStatus() {
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
#endif // INPUTPORT_H
