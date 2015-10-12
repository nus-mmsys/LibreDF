/*
 * 
 *  Tiny Multimedia Framework
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "filters/ehealth/tools/HTTPHandler.h"

HTTPHandler::HTTPHandler(std::string hostName) : hostName(hostName)
{
  port = 80;
}

int HTTPHandler::send(std::string message)
{
    struct hostent * host = gethostbyname(hostName);

    if ( (host == NULL) || (host->h_addr == NULL) ) {
        cout << "Error retrieving DNS information." << endl;
        return -1;
    }

    bzero(&client, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons( port );
    memcpy(&client.sin_addr, host->h_addr, host->h_length);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        cout << "Error creating socket." << endl;
        exit(1);
    }

    if ( connect(sock, (struct sockaddr *)&client, sizeof(client)) < 0 ) {
        close(sock);
        cout << "Could not connect" << endl;
        return -1;
    }

    stringstream ss;
    ss << "POST " + hostName +"/1.0\r\n"
       << "Content-Type: application/x-www-form-urlencoded\r\n"
       << "Connection: close\r\n"
       << "Content-Length: " + std::to_string(message.length()) +"\r\n"
       << "\r\n"
       << message + "\r\n";
       
    string request = ss.str();

    if (send(sock, request.c_str(), request.length(), 0) != (int)request.length()) {
        cout << "Error sending request." << endl;
        return -1;
    }
}


HTTPHandler::~HTTPHandler()
{

}
