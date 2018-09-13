/*
 *   TMF
 *   Copyright (C) TMF Team
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
  class OutputPort: public OPort {
    
    
  private:
    T * data;
    Buffer<T> * buf;
    int index;
    std::vector<IPort *> nextPorts; /**< A list of the next ports. A subclass actor must add its actors to this list */
    
    ClientSocket * sock;
    int chsize;
  public:
    
    /*!
     * OutputPort constructor
     *
     * \param name The name of the output port
     *
     */
    OutputPort<T>(std::string name) : OPort(name), index(0) {
      sock = new ClientSocket("port:"+name);
      buf = new Buffer<T>(8);
      port_cap = std::string(typeid(T).name());
      data = new T();
      chsize = 1024;
    }
    
    virtual int connectPort(std::string host, int portnb) {
	int ret;
	distributed = true;
    	ret = sock->connect(host, portnb);
	if (ret < 0) {
		std::cerr << "port " << name << " cannot connect.\n";
		return ret;
	}
	std::string resp = sock->sendrecv("port");
	if (resp == "continue") {
		increaseLinked();
		return 0;
	}

	try {
		int p = stoi(resp);
		sock->close();
    		ret = sock->connect(host, p);
		if (ret < 0) {
			std::cerr << "port " << name << " cannot connect.\n";
			return ret;
		}
		std::string resp = sock->sendrecv("port");
		if (resp == "continue") {
			increaseLinked();
			return 0;
		}
		return -1;
	} catch (...) {
		std::cerr << "port " << name << " cannot connect.\n";
		return -1;
	}
    }

    int send() {
	char * buf = data->serialize();
	int size = data->getPktSize(buf);
	if (size != chsize)
		chsize = size;
	return sock->send(buf, chsize);
    }

    int getPortNumber() {
	    return sock->getport();
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

    T * get(int i) {
	    return buf->at(i);
    }

    int getBufferSize() {
	    return buf->getSize();
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
    virtual int connectPort(IPort* n) {
      return connectPort(n, nextPorts.size());
    } 

    /*!
     * Add next port to this port specified by an index
     *
     * \param n next port to connect to
     *
     * \param i index of next port 
     */
    virtual int connectPort(IPort* n, int i) {
      int index = i;
      if ( i < 0 || i > nextPorts.size() ) {
	index = nextPorts.size();
      }
      nextPorts.insert(nextPorts.begin()+index, n);
      increaseLinked();
      n->setBuffer(buf);
      return 0;
    } 

    /*!
     * OutputPort desctructor
     *
     */
    virtual ~OutputPort<T>() {
      if (distributed)
	      sock->close();
      delete buf;
      delete data;
      delete sock;
    }
  };
  
}
#endif // DF_OUTPUTPORT_H
