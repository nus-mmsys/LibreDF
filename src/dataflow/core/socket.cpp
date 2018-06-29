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

#include "socket.h"

using namespace df;

Socket::Socket(const std::string& sockname) { 
	name = sockname; 
}

int Socket::listen(int port) {
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

int Socket::accept() {
	int addrlen = sizeof(addr);
	if ((clnsock = ::accept(srvsock, (struct sockaddr*) &addr, (socklen_t*)&addrlen))<0)
    	{
		std::cerr << name << "accept failed.\n";
    		return -1;
	}
	return 0;
}

int Socket::connect(std::string host, int port) {
 	hostaddr = host;
	portnb = port;	
	if ((srvsock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
        	std::cerr << name << " socket creation failed.\n";
		return -1;
	}
  
	std::memset(&addr, '0', sizeof(addr));
  
	addr.sin_family = AF_INET;
	addr.sin_port = htons(portnb);
      
	// Convert IPv4 and IPv6 addres from text to binary form
	if(inet_pton(AF_INET, hostaddr.c_str(), &addr.sin_addr)<=0) 
	{
		std::cerr << name << " invalid addr.\n";
		return -1;
	}
  
	if (::connect(srvsock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		std::cerr << name << " connection failed.\n";
		return -1;
	}
	return 0;
}

void Socket::clnsend(char * buf) {
	::send(clnsock , buf , std::strlen(buf) , 0 );
}

int Socket::clnread(char * buf, int size) {
	return ::read(clnsock , buf, size);
}

void Socket::clnclose() {
	::close(clnsock);
}

void Socket::srvsend(char * buf) {
	::send(srvsock , buf , std::strlen(buf) , 0 );
}

int Socket::srvread(char * buf, int size) {
	return ::read(srvsock , buf, size);
}

void Socket::srvclose() {
	::close(srvsock);
}
