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

#include "df.h"

using namespace df;
using namespace std;

Dataflow::Dataflow(const string& name): name(name), status(DataflowStatus::STOPPED) {
	realtime = false;
	distributed = false;
}

Actor * Dataflow::createActor(std::string const& s, const std::string& name) {
  Actor * res = ActorFactory::createActor(s, name);
  addActor(res);
  return res;
}

void Dataflow::addActor(Actor * f) {
  f->setPipeLock(&io_lock);
  actors.insert(f);
}

void Dataflow::addActors(Actor * f, ...) {
  
  va_list arguments;                     
  
  Actor * cur;
  va_start ( arguments, f);
  
  cur = f;
  while(cur != nullptr) {
    
    addActor(cur);
    cur = va_arg(arguments, Actor*);
  }
  va_end ( arguments ); 
  
}

void Dataflow::connectActors(Actor * src, Actor * snk) {
  src->connectActor(snk);
}

void Dataflow::connectActors(Actor * src, Actor * snk, int p, int c) {
  src->connectActor(snk, p, c);
}

void Dataflow::connectActors(Actor * src, Actor * snk, std::string edge, int p, int c) {
  src->connectActor(snk, edge, p, c);
}

void Dataflow::startDiscovery() {
  tdisc = thread(&Dataflow::discovery, this);
}

void Dataflow::waitDiscovery() {
//  cout << "Discovery terminated...\n";
  tdisc.join();
}

void Dataflow::discovery() {

    //  cout << "Discovery started...\n";

    int opt = 1;
    
    // Creating socket file descriptor
    if ((discsock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
	std::cerr << "discovery socket failed.\n" ;
	exit(EXIT_FAILURE);
    }

    // Attaching socket to the port
    if (setsockopt(discsock, SOL_SOCKET, SO_REUSEADDR,
        &opt, sizeof(opt)) < 0 )
    {
        std::cerr << "discovery setsockopt failed.\n";
       	exit(EXIT_FAILURE);
    }
    discaddress.sin_family = AF_INET;
    discaddress.sin_addr.s_addr = INADDR_ANY;
    discaddress.sin_port = htons( DISCOVERY_PORT );

    /*
    // Forcefully attaching socket to the port
    if (bind(discsock, (struct sockaddr *)&discaddress,
        sizeof(discaddress))<0)
    {
	std::cerr << "discovery bind failed.\n";
        exit(EXIT_FAILURE);
    }
    if (listen(discsock, 3) < 0)
    {
	std::cerr << "discovery listen failed.\n";
        exit(EXIT_FAILURE);
    }

    int addrlen;
    int new_socket;
    char discbuf[1024];

    while (status != DataflowStatus::STOPPED) {
	addrlen = sizeof(discaddress);
	if ((new_socket = accept(discsock, (struct sockaddr*) &discaddress, (socklen_t*)&addrlen))<0)
    	{
		std::cerr << "discovery accept failed.\n";
        	exit(EXIT_FAILURE);
    	}

	int valread = read( new_socket , discbuf, 1024);
        string msg = discbuf;
	send(new_socket, discbuf, strlen(discbuf), 0);
        //handle
	close(new_socket);
    }
    */
}

void Dataflow::init() {
	
  if (!prop.propEmpty("distributed"))
	  distributed = prop.getPropBool("distributed");

  if (!prop.propEmpty("realtime"))
	  realtime = prop.getPropBool("realtime");
	
  for (auto f : actors) {
    f->setProp<bool>("realtime", realtime);
    f->setProp<bool>("distributed", distributed);
  }

  startDiscovery();

  for (auto f: actors) {
    f->startInit();
  }
  
  for (auto f : actors) {
    f->waitInit();
  }
  
  status = DataflowStatus::READY;
  
  for (auto f : actors) {
    
    if (f->getStatus() != ActorStatus::OK)
      status = DataflowStatus::STOPPED;
    return;
  }
  
  
}

void Dataflow::run() {
  
  if (status != DataflowStatus::READY) {
    std::cout << "Dataflow is not ready to run." << endl;
    return;
  }
  
  for (auto f : actors) {
    f->startRun();
  }
  
  status = DataflowStatus::RUNNING;
  
  for (auto f : actors) {
    f->waitRun();
  }
 
  waitDiscovery();

  status = DataflowStatus::STOPPED;
  
}

Dataflow::~Dataflow() {
  
  for (auto f : actors)
    if(f)
      delete f;
    
}
