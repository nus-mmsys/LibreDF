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

#ifndef DF_OUTPUTPORT_H
#define DF_OUTPUTPORT_H

#include "client_socket.h"
#include "port.h"
#include "input_port.h"

namespace df {
  
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
    T * data;
    Buffer<T> * buf;
    int index;
    std::vector<InputPort<T>*> nextPorts; /**< A list of the next ports. A subclass actor must add its actors to this list */
    
    ClientSocket * sock;
    int chsize;
  public:
    
    /*!
     * OutputPort constructor
     *
     * \param name The name of the output port
     *
     */
    OutputPort<T>(std::string name) : Port(name), index(0) {
      sock = new ClientSocket("port:"+name);
      buf = new Buffer<T>();
      port_cap = std::string(typeid(T).name());
      data = new T();
      chsize = 1024;
    }
    
    virtual int connectPort(std::string host, int portnb) {
	return sock->connect(host, portnb);
    }

    void send() {
	char * buf = data->serialize();
	int size = data->getPktSize(buf);
	if (size != chsize)
		chsize = size;
	sock->send(buf, chsize);
    }

    T * getSocketData() {
	return data;
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
      return buf->at(index);
    }
    
    void setStatus(Status st) {
      buf->at(index)->setStatus(st);
    }
    Status getStatus() {
      return buf->at(index)->getStatus();
    } 
    void setSocketStatus(Status st) {
      data->setStatus(st);
    }
    Status getSocketStatus() {
      return data->getStatus();
    }

    /*!
     * Add next port to this port
     *
     * \param n next port to connect to
     */
    virtual int connectPort(Port* n) {
      return connectPort(n, nextPorts.size());
    } 

    /*!
     * Add next port to this port specified by an index
     *
     * \param n next port to connect to
     *
     * \param i index of next port 
     */
    virtual int connectPort(Port* n, int i) {
      int index = i;
      if ( i < 0 || i > nextPorts.size() ) {
	index = nextPorts.size();
      }
      InputPort<T> * in = dynamic_cast<InputPort<T>*>(n);
      nextPorts.insert(nextPorts.begin()+index, in);
      this->increaseLinked();
      in->increaseLinked();	
      in->setBuffer(buf);
      return 0;
    } 

    /*!
     * OutputPort desctructor
     *
     */
    virtual ~OutputPort<T>() {
      sock->close();
      delete buf;
    }
  };
  
}
#endif // DF_OUTPUTPORT_H
