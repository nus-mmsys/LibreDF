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

Dataflow::Dataflow(const string& name): name(name), status(DataflowStatus::NONE) {
	realtime = false;
	distributed = false;
	srvsock = new Socket("df-srv:"+name);
	clnsock = new Socket("df-cln:"+name);
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
	if (distributed) {
		if (dischost == "") {
			cerr << "discovery_host is not specified.\n";
			return;
		}
		if (discport == -1) {
			cerr << "discovery_port is not specified.\n";
			return;
		}

		clnsock->connect(dischost, discport);
		//TODO
		//Ask actor's host and its port number.
		//snkpname, snkhost, snkport, 
		//string snkpname, snkhost;
		//int snkport;
		char msg[1024] = "";
		clnsock->srvsend(msg);
		//src->connectActor(snkpname, snkhost, snkport);
		
	} else {
		src->connectActor(snk, edge, p, c);
	}

}

void Dataflow::startDiscovery() {
  tdisc = thread(&Dataflow::discovery, this);
}

void Dataflow::waitDiscovery() {
  //cout << "Discovery terminated...\n";
  tdisc.join();
}

void Dataflow::discovery() {

  if (!distributed)
	  return;

  if (dischost == "") {
	  cerr << "discovery_host is not specified.\n";
	  return;
  }

  if (discport == -1) {
	  cerr << "discovery_port is not specified.\n";
	  return;
  }
 
  string machine_ip = srvsock->ipaddr("en0");

  if (dischost != machine_ip)
	 return; 

  cout << "Discovery started...\n";
  char buf[1024];
  srvsock->listen(discport);
  
  while (status != DataflowStatus::STOPPED) {
    srvsock->accept();

    srvsock->clnread(buf, 1024);
    //TODO
    //handle message
    //srvsock->clnsend(buf);
    srvsock->clnclose();

  }
  srvsock->srvclose();
    
}

void Dataflow::init() {
	
  if (!prop.propEmpty("distributed"))
	  distributed = prop.getPropBool("distributed");

  if (!prop.propEmpty("realtime"))
	  realtime = prop.getPropBool("realtime");

  if (!prop.propEmpty("discovery_host"))
	  dischost = prop.getProp("discovery_host");

  if (!prop.propEmpty("discovery_port"))
	  discport = prop.getPropInt("discovery_port"); 

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
