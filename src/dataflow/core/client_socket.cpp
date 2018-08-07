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

#include "client_socket.h"

using namespace df;

ClientSocket::ClientSocket(const std::string& sockname) { 
	name = sockname; 
}

int ClientSocket::connect(const std::string& host, int port) {
 	hostaddr = host;
	portnb = port;	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
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
  
	if (::connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		std::cerr << name << " connection failed.\n";
		return -1;
	}
	return 0;
}

std::string ClientSocket::communicate(const std::string& host, int port, const std::string& msg) {
	char buf[1024];
	std::string res;
	std::strcpy(buf, msg.c_str());
	if (connect(host, port) < 0) {
		std::cerr << name << " cannot communicate.\n";
		close();
		return "";
	}
	send(buf, std::strlen(buf));
	recv(buf, 1024);
	res = buf;
	close();
	return res;
}

int ClientSocket::getport() {
	return portnb;
}

std::string ClientSocket::sendrecv(const std::string& msg) {
	int bufsize = 1024;
	char inbuf[bufsize], outbuf[bufsize];
	std::string res;
	std::strcpy(outbuf, msg.c_str());
	send(outbuf, std::strlen(outbuf));
	recv(inbuf, bufsize);
	res = inbuf;
	return res;
}

int ClientSocket::send(const char * buf, int size) {
	return ::send(sock , buf , size , 0 );
}

int ClientSocket::recv(char * buf, int size) {
    	memset(buf, 0, size);
	return ::recv(sock , buf, size, 0);
}

void ClientSocket::close() {
	::close(sock);
}

