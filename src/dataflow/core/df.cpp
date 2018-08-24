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
	logging = true;
	scheduling = true;
	srvsock = new ServerSocket("df-srv:"+name);
	clnsock = new ClientSocket("df-cln:"+name);
}

std::string Dataflow::getName() {
	return name;
}

Dataflow::actor_iter Dataflow::actorBegin() { 
	return actors.begin(); 
} 

Dataflow::actor_iter Dataflow::actorEnd() { 
	return actors.end(); 
} 

int Dataflow::actorSize() { 
	return actors.size(); 
} 
    
Dataflow::edge_iter Dataflow::edgeBegin() { 
	return edges.begin(); 
} 

Dataflow::edge_iter Dataflow::edgeEnd() { 
	return edges.end(); 
} 

int Dataflow::edgeSize() { 
	return edges.size(); 
} 

Edge * Dataflow::createEdge(const std::string& name, const std::string& src,
		      const std::string& snk) {
	Actor * src_actor;
	Actor * snk_actor;

	if (edges.find(name) != edges.end()) {
		std::cerr << "[df.createEdge] error: edge " << name << " already exists.\n";
	       return nullptr;	
	}
	
	if (actors.find(src) != actors.end()) {
		src_actor = actors[src];			
	} else if (remoteactors.find(src) != remoteactors.end()) {
		src_actor = remoteactors[src];
	} else {
		std::cerr << "[df.createEdge] error: cannot find source actor " << src << "\n"; 
		return nullptr;
	}

	if (actors.find(snk) != actors.end()) {
		snk_actor = actors[snk];
	} else if (remoteactors.find(snk) != remoteactors.end()) {
		snk_actor = remoteactors[snk];
	} else {
		std::cerr << "[df.createEdge] error: cannot find sink actor " << snk << "\n"; 
		return nullptr;
	}
	
	Edge * e = new Edge(name);
	
	e->setSource(src_actor);	
	e->setSink(snk_actor);

	e->setSourcePort(src_actor->getSingleOutputPort());
	e->setSinkPort(snk_actor->getSingleInputPort());
			
	edges.insert(std::make_pair(name, e));
	return e;
}

Actor * Dataflow::createActor(std::string const& s, const std::string& name) {
  Actor * res = ActorFactory::createActor(s, name);
  addActor(res);
  return res;
}

Actor * Dataflow::createRemoteActor(const std::string& name) {
	
	Actor * res = ActorFactory::createActor("Remote", name);
	addRemoteActor(res);
	return res;
}

void Dataflow::addActor(Actor * f) {
  f->setIOLock(&iolock);
  actors.insert(std::make_pair(f->getName(), f));
}

void Dataflow::addRemoteActor(Actor * f) {
  f->setIOLock(&iolock);
  remoteactors.insert(std::make_pair(f->getName(), f));
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
	int snkport;
	string snkhost, snkportstr, inp, outp;
	if (distributed) {

		if (remoteactors.find(src->getName()) != remoteactors.end())
			return;

		snkhost = clnsock->communicate(dischost, discport,
				"actor "+snk->getName()+" host");
		snkportstr = clnsock->communicate(dischost, discport,
				"edge "+snk->getName()+" "+edge);
		try {
		    snkport = stoi(snkportstr);
		    outp = edges[edge]->getSourcePort();
		    src->connectActor(outp, snkhost, snkport);
		}
		catch (...) {
		    cerr << snkportstr << " is invalid port number.\n";
		}
	} else {
		outp = edges[edge]->getSourcePort();
		inp = edges[edge]->getSinkPort();
		src->connectActor(snk, outp, inp, p, c);
	}
}

void Dataflow::runDiscovery() {

  if (!prop.propEmpty("distributed"))
	  distributed = prop.getPropBool("distributed");

  if (!prop.propEmpty("discovery_host"))
	  dischost = prop.getProp("discovery_host");

  if (!prop.propEmpty("discovery_port"))
	  discport = prop.getPropInt("discovery_port"); 


  if (!distributed)
	  return;

  string machine_ip = srvsock->ipaddr("en0");

  if (dischost != "127.0.0.1" && dischost != machine_ip)
	 return; 

  cout << "Discovery started...\n";
  srvsock->listen(discport);

  tdisc = thread(&Dataflow::discovery, this);
}

void Dataflow::waitDiscovery() {
  if (!distributed)
	  return;
  tdisc.join();
  cout << "Discovery terminated...\n";
}

void Dataflow::discovery() {
  std::string msg, command, actorname, portname, key, val;
  char buf[1024];
  srvsock->accept();
  while (status != DataflowStatus::STOPPED) {

    srvsock->recv(buf, 1024);
    msg = buf;
    stringstream ss(msg);

    getline(ss, command, ' ');
    getline(ss, actorname, ' ');
    getline(ss, key);

    if (actors.find(actorname) == actors.end()) {
        val = "actor not found";
    }
    else if (command == "actor") {
        val = actors[actorname]->getProp(key);
    }
    else if (command == "edge") {
	portname = edges[key]->getSinkPort();
        val = actors[actorname]->getProp(portname+"_port");
    }
    else if (command == "close") {
    	srvsock->clnclose();
	break;
    }

    strcpy(buf, val.c_str());
    srvsock->send(buf, std::strlen(buf));
    srvsock->clnclose();

    srvsock->accept();
  }
  srvsock->srvclose();    
}

void Dataflow::init() {

  if (!prop.propEmpty("logging"))
	  logging = prop.getPropBool("logging");

  if (!prop.propEmpty("scheduling"))
	  scheduling = prop.getPropBool("scheduling");

  if (!prop.propEmpty("distributed"))
	  distributed = prop.getPropBool("distributed");

  if (!prop.propEmpty("realtime"))
	  realtime = prop.getPropBool("realtime");

  if (!prop.propEmpty("discovery_host"))
	  dischost = prop.getProp("discovery_host");

  if (!prop.propEmpty("discovery_port"))
	  discport = prop.getPropInt("discovery_port"); 

  for (auto f : actors) {
    f.second->setProp<bool>("logging", logging);
    f.second->setProp<bool>("scheduling", scheduling);
    f.second->setProp<bool>("realtime", realtime);
    f.second->setProp<bool>("distributed", distributed);
  }

  if (distributed) {

  	if (dischost == "") {
		cerr << "discovery_host is not specified.\n";
	  	return;
  	}

  	if (discport == -1) {
	  	cerr << "discovery_port is not specified.\n";
	  	return;
  	}
  }

  for (auto f: actors) {
    f.second->startInit();
  }
  
  for (auto f : actors) {
    f.second->waitInit();
  }
  
  status = DataflowStatus::READY;
  
  for (auto f : actors) {
    
    if (f.second->getStatus() != OK)
      status = DataflowStatus::STOPPED;
    return;
  }
  
  
}

void Dataflow::connect() {

	for (auto & ed : edges) {
		df::Edge * e = ed.second;
		df::Actor * src = e->getSource();
		df::Actor * snk = e->getSink(); 
		connectActors(src, snk, ed.first, e->getSourceRate(), e->getSinkRate());
	}

}
void Dataflow::run() {
  
  if (status != DataflowStatus::READY) {
    std::cout << "Dataflow is not ready to run." << endl;
    return;
  }

  int cpunb = std::thread::hardware_concurrency();
  int cpuid = 0; 

  std::cout << "Running the dataflow...\n";
  start = std::chrono::high_resolution_clock::now();

  for (auto f : actors) {
    f.second->startRun(cpuid);
    cpuid = (cpuid + 1) % cpunb;
  }
  
  status = DataflowStatus::RUNNING;
  
  for (auto f : actors) {
    f.second->waitRun();
  }
  
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Execution time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << " ms\n"; 
  /*
  for (auto f : actors) {
	std::cout << f.second->getName() << " = " << f.second->getElapsed() << "\n";
  }
  */

  status = DataflowStatus::STOPPED;

  if (distributed) {
	char msg[8];
	strcpy(msg, "close");
	clnsock->connect(dischost, discport);
	clnsock->send(msg, 8);
	clnsock->close();
  } 
}

Dataflow::~Dataflow() {
  
  for (auto ac : actors)
    if(ac.second)
      delete ac.second;
  for (auto ac : remoteactors)
    if(ac.second)
      delete ac.second;
    
}
