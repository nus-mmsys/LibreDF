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

#include "MediaBuffer.h"
#include "Filter.h"
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
  Port(string name) :
  name(name), linked(0), attrbuf(nullptr), attrindex(0) {
    
  }
  
  /*!
   * Get the name of the port
   *
   * \return the name of the port.
   */
  string getName() {
    return name;
  }
  
  const PortCaps & getPortCaps() const {
  
    return portCaps;
  }
  /*!
   * Get the number of the ports connected to this port
   *
   * \return the number of the port.
   */
  int getLinked() {
    return linked;
  }
  
  /*!
   * Increase the number of the ports linked to the port.
   * (The filter uses this function when it connects two filters)
   *
   */
  void increaseLinked() {
    linked++;
  }
  
  Attribute * getAttr() {
    return attrbuf->at(attrindex)->get();
  }
  
  virtual void connectPort(Port* n) {}
  
  /*!
   * Port descructor
   *
   */
  virtual ~Port() {
  }
};

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
  InputPort<T>(string name) : Port(name), buf(nullptr), index(0) {
    portCaps.addCaps("template", string(typeid(T).name()));
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
  vector<InputPort<T>*> nextPorts; /**< A list of the next ports. A subclass filter must add its filters to this list */
  
  
public:
  
  /*!
   * OutputPort constructor
   *
   * \param name The name of the output port
   *
   */
  OutputPort<T>(string name) : Port(name), index(0) {
    buf = new MediaBuffer<T>();
    attrbuf = new MediaBuffer<Attribute>();
    portCaps.addCaps("template", string(typeid(T).name()));
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

#endif /* PORT_H_ */
