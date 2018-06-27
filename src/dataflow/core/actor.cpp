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

#include "actor.h"

using namespace df;
using namespace std;

Actor::Actor(const string &name) : status(ActorStatus::OK), stepno(0), name(name) {
  realtime = false;
  distributed = false;
  home_path = std::getenv("HOME");
  df_path = home_path + "/Documents/df/";  
  dfout_path = df_path + "outputs/";
}

std::string Actor::getName() {
	return name;
}
void Actor::log(std::string msg) {
  dataflowlock->lock(); 
  std::cout << name << ": " << msg << std::endl;
  dataflowlock->unlock();
}
void Actor::sleep(int s) {
  this_thread::sleep_for(chrono::milliseconds{rand()%s});
}

void Actor::connectActor(Actor * snk) {
  connectActor(snk,1,1);  
}

int Actor::connectActor(std::string portname, std::string host, int portnb) {
  
  if (outputPorts.find(portname) == outputPorts.end())
    return -1;

  return outputPorts[portname]->connectPort(host, portnb);

}

void Actor::connectActor(Actor * snk, int p, int c) {
  
  bool linked = false;
  for (auto fout : outputPorts) {
    for (auto fin : snk->inputPorts) {
      
      string typeOut = fout.second->getPortCap();
      string typeIn = fin.second->getPortCap();
      
      if ( fin.second->getLinked() == 0 && (typeOut == typeIn)) {
	
	fout.second->connectPort(fin.second, p, c);
	
	linked = true;
	break;
      }
    }
    if (linked)
      break;
  }
}

int Actor::connectActor(Actor * snk, std::string outp, std::string inp, int p, int c) {
	
  Port *in, *out;
  if (outputPorts.find(outp) == outputPorts.end()) {
	  log("port "+outp+" is not found.");
 	  return -1; 
  } else
	out = outputPorts[outp];

  if (snk->inputPorts.find(inp) == snk->inputPorts.end()) {
	  log("port "+inp+" of actor "+snk->getName()+" is not found.");
	  return -1;
  } else
	in = snk->inputPorts[inp];
 
  if ( in->getLinked() == 0 ) {
	 log("port "+inp+" is already linked.");
	 return -1;
  }

  string type_out = out->getPortCap();
  string type_in = in->getPortCap();
 
  if (type_out != type_in) {
	log("types of ports "+inp+" and "+outp+" do not match.");
	return -1;
  }	

  return out->connectPort(in, p, c);

}
	
void Actor::startInit() {
  tinit = thread(&Actor::initActor, this);
}

void Actor::startRun() {
  trun = thread(&Actor::runActor, this);
}

void Actor::waitInit() {
  tinit.join();
}

void Actor::waitRun() {
  trun.join();
}

void Actor::setPipeLock(mutex * mux) {
  dataflowlock = mux;
}

void Actor::initActor() {

  if (!propEmpty("realtime"))
	  realtime = getPropBool("realtime");

  if (!propEmpty("distributed"))
	  distributed = getPropBool("distributed");

  for (auto p : inputPorts) {
    if (distributed)
	    listen(p.second);
    else if (p.second->getLinked() == 0) {
      log(p.second->getName()+string(" is not connected"));
      status = ActorStatus::ERROR;
    }
  }
 
  //TODO 
  //Here actor should contact tmf-discovery
  //to ask which port to connect to. 
  for (auto p : outputPorts) {
    if (p.second->getLinked() == 0) {
      log(p.second->getName()+string(" is not connected"));
      status = ActorStatus::ERROR;
    }
  }
  //////
   
  init();
}

void Actor::runActor() {
  
  while(status != ActorStatus::EOS) {
    realtime? runRT() : run();
    stepno++;
  }
  
}

Actor::~Actor() { 
}
