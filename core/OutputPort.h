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

#ifndef OUTPUTPORT_H
#define OUTPUTPORT_H

#include "core/Port.h"
#include "core/InputPort.h"

namespace tmf {
  
  /*!
   * \class OutputPort
   *
   * OutputPort class is a subclass of the Port class.
   * It is a class template and the type of the buffer of the port is a template.
   *
   */
  
  template <typename T>
  class OutputPort: public Port {
    
    
  private:
    MediaBuffer<T> * buf;
    int index;
    std::vector<InputPort<T>*> nextPorts; /**< A list of the next ports. A subclass filter must add its filters to this list */
    
    
  public:
    
    /*!
     * OutputPort constructor
     *
     * \param name The name of the output port
     *
     */
    OutputPort<T>(std::string name) : Port(name), index(0) {
      buf = new MediaBuffer<T>();
      attrbuf = new MediaBuffer<Attribute>();
      portCaps.addCaps("template", std::string(typeid(T).name()));
    }
    
    void lockAttr() {
      attrbuf->at(attrindex)->producerLock();
    }
    
    void unlockAttr() {
      attrbuf->at(attrindex)->producerUnlock();
      attrindex = (attrindex+1) % attrbuf->getSize();
    }
    
    void lock() {
      buf->at(index)->producerLock();
    }
    
    bool lockRT() {
      return buf->at(index)->producerRTLock();
    }
    
    void unlock() {
      
      buf->at(index)->producerUnlock();
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
     * Add next port to this port
     *
     * \param n next port to connect to
     */
    virtual void connectPort(Port* n) {
      
      InputPort<T> * in = dynamic_cast<InputPort<T>*>(n);
      nextPorts.push_back(in);
      this->increaseLinked();
      in->increaseLinked();	
      in->setBuffer(buf);
      in->setAttrBuffer(attrbuf);
    } 
    
    /*!
     * OutputPort desctructor
     *
     */
    virtual ~OutputPort<T>() {
      delete buf;
      delete attrbuf;
    }
  };
  
}
#endif // OUTPUTPORT_H
