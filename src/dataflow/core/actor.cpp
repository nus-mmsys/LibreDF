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

void Actor::log(std::string msg) {
  dataflowlock->lock(); 
  std::cout << name << ": " << msg << std::endl;
  dataflowlock->unlock();
}
void Actor::sleep(int s) {
  this_thread::sleep_for(chrono::milliseconds{rand()%s});
}

void Actor::connectActor(Actor * f) {
  connectActor(f,1,1);  
}

void Actor::connectActor(std::string portname, std::string host, int portnb) {
  
  for (auto fout : outputPorts) {
    	  if ( fout->getName() == portname) {
		fout->connectPort(host, portnb);
		break;
	  }
  }
}

void Actor::connectActor(Actor * f, int p, int c) {
  
  bool linked = false;
  for (auto fout : outputPorts) {
    for (auto fin : f->inputPorts) {
      
      string typeOut = fout->getPortCap();
      string typeIn = fin->getPortCap();
      
      if ( fin->getLinked() == 0 && (typeOut == typeIn)) {
	
	fout->connectPort(fin, p, c);
	
	linked = true;
	break;
      }
    }
    if (linked)
      break;
  }
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
	    listen(p);
    else if (p->getLinked() == 0) {
      log(p->getName()+string(" is not connected"));
      status = ActorStatus::ERROR;
    }
  }
  
  for (auto p : outputPorts) {
    if (p->getLinked() == 0) {
      log(p->getName()+string(" is not connected"));
      status = ActorStatus::ERROR;
    }
  } 
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
