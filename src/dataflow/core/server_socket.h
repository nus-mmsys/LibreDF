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

#ifndef DF_SERVERSOCKET_H
#define DF_SERVERSOCKET_H

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
  
  class ServerSocket {
    
  private:

     std::string name;
     int srvsock;
     int clnsock;
     std::string hostaddr;
     int portnb;
     struct sockaddr_in addr;

  public:
    
    ServerSocket(const std::string& sockname); 

    int getport();
    int listen(int port); 
    int accept();     
    int send(char * buf, int size); 
    int recv(char * buf, int size); 
    int recvwait(char * buf, int size); 
    int recvpeek(char * buf, int size); 
    void clnclose(); 
    void srvclose();
    void recvsend(const std::string& inmsg, const std::string& outmsg);
    std::string ipaddr(const std::string& interface);
  };
}

#endif // DF_SERVERSOCKET_H
