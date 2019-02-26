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
    std::vector<T *> datavec;
    Buffer<T> * buf;
    char * sendbuf;
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
      buf = new Buffer<T>(16);
      port_cap = std::string(typeid(T).name());
      data = new T();
      for (int i=0; i<rate; i++)
	      datavec.push_back(new T());
      chsize = 1024;
    }

    virtual void setBufferSize(int s) {
      delete buf;
      buf = new Buffer<T>(s);
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
	sendbuf = data->serialize();
	int size = data->getPktSize(sendbuf);
	if (size != chsize)
		chsize = size;
	return sock->send(sendbuf, chsize);
    }

    int sendMR() {
	int res=0;
	for(int i=0; i<rate; i++) {
	   sendbuf = datavec[i]->serialize();
	   int size = data->getPktSize(sendbuf);
	   if (size != chsize)
		    chsize = size;
	   res = sock->send(sendbuf, chsize);
	   if (res<0)
		   return res;
	}
	return res;
    }

    int getPortNumber() {
	    return sock->getport();
    }

    T * getSocketData() {
	return data;
    }

    std::vector<T *> getSocketDataMR() {
	return datavec;
    }

    void lock() {
      buf->at(index)->producerLock();
    }
    
    int lockMR() {
      if (rate >= buf->getSize())
	      return -1;
      for (int i=0; i<rate; i++) {
	      buf->at((index+i)%buf->getSize())->producerLock();
      }
      return 0;
    }

    bool lockRT() {
      return buf->at(index)->producerRTLock();
    }
    
    void unlock() {
      
      buf->at(index)->producerUnlock();
      index = (index+1) % buf->getSize();
    }
     
    int unlockMR() {
      if (rate >= buf->getSize())
	      return -1;
      for (int i=0; i<rate; i++) {
	      buf->at((index+i)%buf->getSize())->producerUnlock();
      }
      index = (index+rate) % buf->getSize();
      return 0;
    }

    T * get() {
      return buf->at(index);
    }
 
    std::vector<T *> getMR() {
	std::vector<T *> res;
	if (rate >= buf->getSize())
		return res;
	for (int i=0; i<rate; i++)
		res.push_back(buf->at((index+i)%buf->getSize()));
    	return res;
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
    
    int setStatusMR(Status st) {
      	if (rate >= buf->getSize())
		return -1;;
      	for (int i=0; i<rate; i++) {
      		buf->at((index+i)%buf->getSize())->setStatus(st);
      	}
	return 0;
    }

    Status getStatusMR() {
      Status res = getStatus();
      if (rate >= buf->getSize())
	      return ERROR;
      for (int i=0; i<rate; i++) {
	      res = buf->at((index+i)%buf->getSize())->getStatus();
	      if (res != OK)
		      return res;
      }
      return res;
    }

    void setSocketStatus(Status st) {
      data->setStatus(st);
    }

    void setSocketStatusMR(Status st) {
      datavec[datavec.size()-1]->setStatus(st);
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
      int idx = i;
      if ( i < 0 || i > nextPorts.size() ) {
	idx = nextPorts.size();
      }
      nextPorts.insert(nextPorts.begin()+idx, n);
      increaseLinked();
      n->setBuffer(buf, index);
      return 0;
    } 

    /*!
     * Disconnect next port from this port
     *
     * \param n next port to disconnect from
     *
     */
    virtual int disconnectPort(IPort* n) {
      nextPorts.clear();
      clearLinked();
      n->clearBuffer();
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
