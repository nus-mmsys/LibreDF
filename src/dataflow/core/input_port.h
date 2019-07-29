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
  class InputPort: public IPort {
    
  private:
    T * data;
    std::vector<T *> datavec;
    Buffer<T> * buf;
    int index;
    char * chdata;
    int chsize;

  public:
    
    /*!
     * InputPort constructor
     *
     * \param name The name of the port
     *
     */
    InputPort<T>(std::string name) : IPort(name), buf(nullptr), index(0) {
        port_cap = std::string(typeid(T).name());
	data = new T();
	for (int i=0; i<rate; i++)
		datavec.push_back(new T());
	chsize = 1024;
	chdata = new char[chsize];
    }
    
    virtual void listen(int portnb) {
	distributed = true;
        sock->listen(portnb);
    }

    virtual void startAccept() {
        taccept = std::thread(&InputPort<T>::accept, this);
    }

    virtual void waitAccept() {
        taccept.join();
    }

    virtual void accept() {
	sock->accept();
	sock->recvsend("port", "continue");
	increaseLinked();
    } 
    
    T * recv() {
    	while (sock->recvpeek(chdata, sizeof(int)) < 0);

	int size = data->getPktSize(chdata);
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
 
    std::vector<T *> recvMR() {
	for (int i=0; i<rate; i++) {
    	    while (sock->recvpeek(chdata, sizeof(int)) < 0);

	    int size = datavec[i]->getPktSize(chdata);
	    if (size != chsize) {
	  	    chsize = size;
		    delete chdata;
		    chdata = new char[size];
	    }
    	    if (sock->recvwait(chdata, chsize) < 0)
		    return std::vector<T *>();
	    datavec[i]->deserialize(chdata);
	}
	return datavec;
    }

    int getPortNumber() {
	    return sock->getport();
    }

    void setBuffer(BufferInfc * b, int & inpidx, int i) {
      buf = static_cast<Buffer<T> *>(b);
      buf->addConsumer();
      increaseLinked();	
      inpidx = 0;
      index = i;
    }
 
    void unsetBuffer(int inpidx) {
    	clearBuffer();
    }
   
    virtual void clearBuffer() {
      if (buf != nullptr)
	      buf->clearConsumer();
      buf = nullptr;
      clearLinked();	
    }

    void lock() {
      buf->at(index)->consumerLock();
    }
    
    int lockMR() {
	if (rate >= buf->getSize())
		return -1;
      	for (int i=0; i<rate; i++) {
		buf->at((index+i)%buf->getSize())->consumerLock();
      	}
	return 0;
    }

    void unlock() {
      buf->at(index)->consumerUnlock();
      index = (index+1) % buf->getSize();
    }
     
    int unlockMR() {
	if (rate >= buf->getSize())
		return -1;
      	for (int i=0; i<rate; i++) {
      		buf->at((index+i)%buf->getSize())->consumerUnlock();
      	}
	index = (index+rate)%buf->getSize();
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
   
    void setStatus(Status st) {
      buf->at(index)->setStatus(st);
    }

    Status getStatus() {
      return buf->at(index)->getStatus();
    }

    int setStatusMR(Status st) {
      	if (rate >= buf->getSize())
		return -1;;
      	buf->at((index+rate-1)%buf->getSize())->setStatus(st);
	return 0;
    }

    Status getStatusMR() {
      if (rate >= buf->getSize())
	      return ERROR;
      return buf->at((index+rate-1)%buf->getSize())->getStatus();
    }

    void setSocketStatus(Status st) {
      data->setStatus(st);
    }

    Status getSocketStatus() {
      return data->getStatus();
    }

    /*!
     * Get buffer occupancy
     *
     */
    int getOccupancy(int idx) {
	return index;
    }

    /*!
     * InputPort destructor
     *
     */
    virtual ~InputPort() {
	    if (distributed) {
	    	sock->clnclose();
	    	sock->srvclose();
	    }
	    delete data;
	    delete chdata;
    }
    
  };
  
}
#endif // DF_INPUTPORT_H
