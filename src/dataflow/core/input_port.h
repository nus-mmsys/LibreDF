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

#ifndef DF_INPUTPORT_H
#define DF_INPUTPORT_H

#include <thread>

#include "server_socket.h"
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

    T * data;
    std::thread taccept;
    Buffer<T> * buf;
    int index;
    ServerSocket * sock;
    char * chdata;
    int chsize;

  public:
    
    /*!
     * InputPort constructor
     *
     * \param name The name of the port
     *
     */
    InputPort<T>(std::string name) : Port(name), buf(nullptr), index(0) {
	sock = new ServerSocket("port:"+name);
        port_cap = std::string(typeid(T).name());
	data = new T();
	chsize = 1024;
	chdata = new char[chsize];
    }
   
    virtual void listen(int portnb) {
	sock->listen(portnb);
    } 

    void accept() {
	sock->accept();
    } 

    virtual void startAccept() {
        taccept = std::thread(&InputPort<T>::accept, this);
    }

    virtual void waitAccept() {
        taccept.join();
    }

    T * recv() {
    	if (sock->recvpeek(chdata, sizeof(int)) < 0)
	        return nullptr;
	int size = data->pktsize(chdata)+sizeof(int);
	if (size != chsize) {
		chsize = size;
		delete chdata;
		chdata = new char[size];
	}
    	if (sock->recvwait(chdata, chsize) < 0)
		return nullptr;
	data->deserialize(chdata);
	return data;
    }

    void setBuffer(Buffer<T> * b) {
      buf = b;
      buf->addConsumer();
    }
    
    void lock() {
      buf->at(index)->consumerLock();
    }
    
    void unlock() {
      buf->at(index)->consumerUnlock();
      index = (index+1) % buf->getSize();
    }
    
    T * get() {
    	return buf->at(index);
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
	    delete data;
	    delete chdata;
    }
    
  };
  
}
#endif // DF_INPUTPORT_H
