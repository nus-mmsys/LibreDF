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

#include "server_socket.h"

using namespace df;

ServerSocket::ServerSocket(const std::string& sockname) { 
	name = sockname; 
}

int ServerSocket::listen(int port) {
	portnb = port;

    	int opt = 1;

    	// Creating socket file descriptor
	if ((srvsock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << name << " socket failed.\n" ;
		return -1;
	}

	// Attaching socket to the port
        if (setsockopt(srvsock, SOL_SOCKET, SO_REUSEADDR,
                                                  &opt, sizeof(opt)) < 0 )
        {
        	std::cerr << name << " setsockopt failed.\n";
        	return -1;
	}
    	addr.sin_family = AF_INET;
    	addr.sin_addr.s_addr = INADDR_ANY;
    	addr.sin_port = htons( portnb );

    	// Forcefully attaching socket to the port
    	if (bind(srvsock, (struct sockaddr *)&addr,
                                 sizeof(addr))<0)
    	{
		std::cerr << name << "bind failed.\n";
    		return -1;
	}
    	if (::listen(srvsock, 3) < 0)
    	{
		std::cerr << name << "listen failed.\n";
    		return -1;
	}

    	return 0;
} 

int ServerSocket::accept() {
	int addrlen = sizeof(addr);
	if ((clnsock = ::accept(srvsock, (struct sockaddr*) &addr, (socklen_t*)&addrlen))<0)
    	{
		std::cerr << name << "accept failed.\n";
    		return -1;
	}
	return 0;
}

void ServerSocket::send(char * buf, int size) {
	::send(clnsock , buf , size , 0 );
}

int ServerSocket::recv(char * buf, int size) {
	memset(buf, 0, size);
	return ::recv(clnsock , buf, size, 0);
}

void ServerSocket::clnclose() {
	::close(clnsock);
}

void ServerSocket::srvclose() {
	::close(srvsock);
}

std::string ServerSocket::ipaddr(const std::string& interface){
	std::string ipAddress="";
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    // retrieve the current interfaces - returns 0 on success
    success = getifaddrs(&interfaces);
    if (success == 0) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while(temp_addr != NULL) {
            if(temp_addr->ifa_addr->sa_family == AF_INET) {
                if(std::strcmp(temp_addr->ifa_name, interface.c_str())==0){
                    ipAddress=inet_ntoa(((struct sockaddr_in*)temp_addr->ifa_addr)->sin_addr);
                }
            }
            temp_addr = temp_addr->ifa_next;
        }
    }
    // Free memory
    freeifaddrs(interfaces);
    return ipAddress;
}
