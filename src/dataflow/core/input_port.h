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
    int new_socket;

  public:
    
    /*!
     * InputPort constructor
     *
     * \param name The name of the port
     *
     */
    InputPort<T>(std::string name) : Port(name), buf(nullptr), index(0) {
	port_cap = std::string(typeid(T).name());
    }
   
    void listenPort(int portnb) {
	port_nb = portnb;

    	int opt = 1;

    	// Creating socket file descriptor
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		std::cerr << "port " << name << " socket failed.\n" ;
		exit(EXIT_FAILURE);
	}

	// Attaching socket to the port
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
        {
        	std::cerr << "port " << name << "setsockopt failed.\n";
       		exit(EXIT_FAILURE);
        }
    	address.sin_family = AF_INET;
    	address.sin_addr.s_addr = INADDR_ANY;
    	address.sin_port = htons( port_nb );

    	// Forcefully attaching socket to the port
    	if (bind(sock, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    	{
		std::cerr << "port " << name << "bind failed.\n";
        	exit(EXIT_FAILURE);
    	}
    	if (listen(sock, 3) < 0)
    	{
		std::cerr << "port " << name << "listen failed.\n";
        	exit(EXIT_FAILURE);
    	}

    } 

    void acceptPort() {
	if ((new_socket = accept(sock, (struct sockaddr *)&address, 
                       sizeof(address)))<0)
    	{
		std::cerr << "port " << name << "accept failed.\n";
        	exit(EXIT_FAILURE);
    	}
    }

    char * readPort() {
	int valread = read( new_socket , sock_buf, 1024);
	char * res = sock_buf;
	return res;	
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
