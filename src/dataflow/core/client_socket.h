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

#ifndef DF_CLIENTSOCKET_H
#define DF_CLIENTSOCKET_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ifaddrs.h>

#include <cstring>
#include <iostream>
#include <string>

namespace df {
  
  /*!
   * \class Socket
   *
   */
  
  class ClientSocket {
    
  private:

     std::string name;
     int sock;
     std::string hostaddr;
     int portnb;
     struct sockaddr_in addr;

  public:
    ClientSocket(const std::string& sockname); 

    int connect(const std::string& host, int port); 
    std::string communicate(const std::string& host, int port, const std::string& msg);
    std::string sendrecv(const std::string& msg);
    void send(const char * buf, int size); 
    int recv(char * buf, int size); 
    void close();

  };
}

#endif // DF_CLIENTSOCKET_H
