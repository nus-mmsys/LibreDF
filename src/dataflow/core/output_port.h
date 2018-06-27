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
    Buffer<T> * buf;
    int index;
    std::vector<InputPort<T>*> nextPorts; /**< A list of the next ports. A subclass actor must add its actors to this list */
    
    std::string host_addr;

  public:
    
    /*!
     * OutputPort constructor
     *
     * \param name The name of the output port
     *
     */
    OutputPort<T>(std::string name) : Port(name), index(0) {
      buf = new Buffer<T>();
      port_cap = std::string(typeid(T).name());
    }
    
    virtual int connectPort(std::string host, int portnb) {
 	host_addr = host;
	port_nb = portnb;	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
        	std::cerr << "port " << name << " socket creation failed.\n";
		return -1;
	}
  
	std::memset(&address, '0', sizeof(address));
  
	address.sin_family = AF_INET;
	address.sin_port = htons(port_nb);
      
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, host_addr.c_str(), &address.sin_addr)<=0) 
	{
		std::cerr << "port " << name << " invalid address.\n";
		return -1;
	}
  
	if (connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cerr << "port " << name << " connection failed.\n";
		return -1;
	}
	return 0;
    }

    void send(char * buf) {
	send(sock , buf , std::strlen(buf) , 0 );
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
    
    void setStatus(TokenStatus st) {
      buf->at(index)->setStatus(st);
    }
    TokenStatus getStatus() {
      return buf->at(index)->getStatus();
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
      InputPort<T> * in = dynamic_cast<InputPort<T>*>(n);
      nextPorts.insert(nextPorts.begin()+i, in);
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
      delete buf;
    }
  };
  
}
#endif // DF_OUTPUTPORT_H
