/*
 *   LibreDF
 *   Copyright (C) A. Shafiei
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

Actor::Actor(const string &name) : status(OK), stepno(1), name(name) {
  realtime = false;
  distributed = false;
  logging = true;
  scheduling = true;
  paused = false;
  iter_max = 0;
  solution = 1;
  iterno = 1;
  fireno = 1;
  cpuid = 0;
  start_iter = 0;
  end_iter = 0;
  start_firing = 0;
  end_firing = 0;
  start_exec = 0;
  end_exec = 0;
  exec_dur = 0;
  average_period = 1000;
  instance_period = 1000;
}

std::string Actor::getName() {
	return name;
}

std::string Actor::getType() {
	return getProp("computation");
}

void Actor::setType(std::string t) {
	type = t;
	setProp("computation", t);
}

void Actor::setCpuId(int cpu) {
	cpuid = cpu;
}

int Actor::getCpuId() {
	return cpuid;
}

void Actor::setSolution(int sol) {
	solution = sol;
}

int Actor::getSolution() {
	return solution;
}

bool Actor::setProps(std::map<std::string, std::string> p) {
	bool rep = false;
	for (auto && v : p) {
		if (p.find(v.first) == p.end()) {
			prop.setProp(v.first, v.second);
			rep = true;
		} else if (getProp(v.first) != v.second) {
			prop.replaceProp(v.first, v.second);
			rep = true;
		}
	}
	return rep;
}

void Actor::execlog() {
  string s = name + " : "
	  + to_string(stepno) + " " 
	  + to_string(iterno) + " " ;
	  //+ to_string(start_firing) + " "
	  //+ to_string(end_firing) + " " 
	  //+ to_string(end_firing-start_firing);
  if (start_exec != 0) {
	s = s + to_string(start_exec) + " "
	      + to_string(end_exec) + " "
	      + to_string(end_exec - start_exec);
  }
  s+= "\n";
  if (logging) {
     iolock->lock();
     std::cout << s;
     iolock->unlock();
  } else {
     logfile << s;     
  }
}
void Actor::log(std::string msg) {
  if (logging) {
    string s = name + " : [" + to_string(iterno) + ", " + to_string(fireno) + "] [" + timer.endUs() + "] [" + to_string(sched_getcpu()) + "] " + msg + "\n";
    iolock->lock(); 
    std::cout << s;
    iolock->unlock();
  }
}

std::string Actor::getSingleInputPort() {
	if (inputPorts.size() == 1)
		return inputPorts.begin()->second->getName();
	return "";
}

std::string Actor::getSingleOutputPort() {
	if (outputPorts.size() == 1)
		return outputPorts.begin()->second->getName();
	return "";
}

int Actor::setInputPortRate(const string& portname, int rate) {
	if (inputPorts.size() == 1) {
		inputPorts.begin()->second->setRate(rate);
		return 0;
	}

	if (inputPorts.find(portname) == inputPorts.end()) {
		return -1;
	}

	inputPorts[portname]->setRate(rate);
	return 0;
}

int Actor::setOutputPortRate(const string& portname, int rate) {
	if (outputPorts.size() == 1) {
		outputPorts.begin()->second->setRate(rate);
		return 0;
	}

	if (outputPorts.find(portname) == outputPorts.end()) {
		return -1;
	}

	outputPorts[portname]->setRate(rate);
	return 0;
}

int Actor::connectActor(Actor * snk) {
  return connectActor(snk,1,1);  
}

int Actor::connectActor(std::string outp, std::string host, int portnb) {

  if (outputPorts.find(outp) == outputPorts.end()) {
	  log("port "+outp+" is not found.");
 	  return -1; 
  }  

  if (outputPorts.find(outp) == outputPorts.end())
    return -1;

  int ret = outputPorts[outp]->connectPort(host, portnb);
  return ret;
}

int Actor::connectActor(Actor * snk, int p, int c) {
  
  bool linked = false;
  for (auto fout : outputPorts) {
    for (auto fin : snk->inputPorts) {
      
      string typeOut = fout.second->getPortCap();
      string typeIn = fin.second->getPortCap();
      
      if ( fin.second->getLinked() == 0 && (typeOut == typeIn)) {
	
	fout.second->connectPort(fin.second);
	fout.second->setRate(p);
	fin.second->setRate(c);
	
	linked = true;
	break;
      }
    }
    if (linked)
      break;
  }
  return 0;
}

int Actor::connectActor(Actor * snk, std::string outp, std::string inp, int & outpidx, int & inpidx, int p, int c) {
	
  IPort * in;
  OPort * out;

  if (outp == "" && inp == "")
	  return connectActor(snk, p, c);

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

  /* 
  if ( in->getLinked() > 0 ) {
	 log("port "+inp+" is already linked.");
	 return -1;
  }
  */

  string type_out = out->getPortCap();
  string type_in = in->getPortCap();
 
  if (type_out != type_in) {
	log("types of ports "+inp+" and "+outp+" do not match.");
	return -1;
  }	

  int ret = out->connectPort(in, outpidx, inpidx);
  out->setRate(p);
  in->setRate(c);
  return ret;

}

int Actor::disconnectActor(Actor * snk, std::string outp, std::string inp, int outpidx, int inpidx) {
  IPort * in;
  OPort * out;

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
  int ret = out->disconnectPort(in, outpidx, inpidx);
  return ret;
}

void Actor::startInit() {
  tinit = thread(&Actor::initActor, this);
}

void Actor::startReInit() {
  treinit = thread(&Actor::reInitActor, this);
}

void Actor::startRun() {

  struct sched_param sch_param;

  if (!propEmpty("cpu")) {
	  cpuid = getPropInt("cpu");
  }
  
  trun = thread(&Actor::runActor, this);
  //sch_param.sched_priority = sched_get_priority_max(SCHED_RR);
  //sched_setscheduler(trun.native_handle(), SCHED_RR, &sch_param);	

  if (scheduling) { 
  	cpu_set_t cpuset;
  	CPU_ZERO(&cpuset);
  	CPU_SET(cpuid, &cpuset);

  	int rc = pthread_setaffinity_np(trun.native_handle(), sizeof(cpu_set_t), &cpuset);
  	if (rc != 0)
		log("error calling pthread_setaffinity_np");
  }
}

void Actor::startResumeTill(int iter) {
  tresumetill = thread(&Actor::resumeTill, this, iter);
}

void Actor::waitResumeTill() {
  tresumetill.join();
}

void Actor::startPause() {
  tpause = thread(&Actor::pause, this);
}

void Actor::waitPause() {
  tpause.join();
}

void Actor::waitInit() {
  tinit.join();
}

void Actor::waitReInit() {
  treinit.join();
}

void Actor::waitRun() {
  trun.join();

}

void Actor::setIOLock(mutex * mux) {
  iolock = mux;
}

void Actor::initActor() {

  init();

  if (!propEmpty("logging"))
	  logging = getPropBool("logging");

  if (!propEmpty("scheduling"))
	  scheduling = getPropBool("scheduling");

  if (!propEmpty("realtime"))
	  realtime = getPropBool("realtime");

  if (!propEmpty("distributed"))
	  distributed = getPropBool("distributed");

  for (auto p : outputPorts) {
	if (!propEmpty(p.first+"_size")) {
		p.second->setBufferSize(getPropInt(p.first+"_size"));
	}
  }
  
  if (distributed) {
    for (auto p : inputPorts) {
	    listen(p.second);
	    p.second->startAccept();
    }
  }

  if (!logging) {
  	logfile.open(fsys.logPath()+name);
  }

}

void Actor::reInitActor() {

  reinit();

  /*
  for (auto p : outputPorts) {
	if (!propEmpty(p.first+"_size")) {
		p.second->setBufferSize(getPropInt(p.first+"_size"));
	}
  }
  */
  
  /*
  if (distributed) {
    for (auto p : inputPorts) {
	    listen(p.second);
	    p.second->startAccept();
    }
  }
  */
   
}

void Actor::listen(IPort * port) {

      std::string name = port->getName();
      if (propEmpty(name+"_port")) {
	      log(name+"_port is not specified.");
      } 
      else {
	      int portnb = getPropInt(name+"_port");
      	      port->listen(portnb);
      }
}

void Actor::assignTime() {
	if (inputPorts.empty())
		setTime(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}

unsigned long Actor::getTime() {
	unsigned long res;
	time_mux.lock();
	res = creation_time;
	time_mux.unlock();
	return res;
}

unsigned long Actor::getStopWatch() {
	return timer.now()-stime;
}

void Actor::setTime(unsigned long c) {
	time_mux.lock();
	creation_time = c;
	time_mux.unlock();
}

void Actor::runIter() {
	//start_iter = timer.nowUs();
	for (int i=0; i<solution; i++) {

	  assignTime();

	  start_firing = timer.nowUs();
	  fireno = i+1;
	  if (realtime) {
	  	runRT();
    	  } else {
		run();
    	  }

	  instance_period = timer.nowUs() - end_firing;
	  if (instance_period < 1000 || instance_period > 2500000)
		  instance_period = 1000;
	  average_period = 0.95*average_period + 0.05*instance_period;

	  end_firing = timer.nowUs();
	  
	  latency = end_firing - getTime();

	  execlog();
          stepno++;
	}
	//end_iter = timer.nowUs();
	//iterlog();
	iterno++;
}

void Actor::runActor() {
  if (!distributed) {
    for (auto p : inputPorts) {
      if (p.second->getLinked() == 0) {
        log(p.second->getName()+string(" is not connected"));
      	setStatus(ERROR);
      }
    }
    for (auto p : outputPorts) {
      if (p.second->getLinked() == 0) {
        log(p.second->getName()+string(" is not connected"));
        setStatus(ERROR);
      }
    }
  }
  if (distributed) {
    for (auto p : inputPorts) {
	    p.second->waitAccept();
    }
  }

  start();
  
  timer.start();
  stime = timer.now();
  while(getStatus() != EOS) {
	  
      {
	  lock_guard<mutex> lockrun(runend_mux);
	  runIter();
          sched_yield();	  
      }

      {
	  unique_lock<mutex> lockpause(pause_mux);
	  while(paused && iterno==iter_max)
		  pause_cond.wait(lockpause);
      }
  }
}

int Actor::pause() {
    lock_guard<mutex> lockpause(pause_mux);
    lock_guard<mutex> lockrun(runend_mux);
    int res = iterno;
    iter_max = iterno;
    paused = true;
    //pause_cond.notify_all();
    return res;
}

void Actor::pauseAfter(int imax) {
    {
    	lock_guard<mutex> lockpause(pause_mux);
    	paused = true;
	iter_max = imax;
    }
    pause_cond.notify_one();
}

bool Actor::isPaused() {
    while(true)
    {
    	lock_guard<mutex> lockpause(pause_mux);
    	if (paused and iterno==iter_max)
		return true;
    }
    return false;
}

void Actor::resume() {
    {
    	lock_guard<mutex> lockpause(pause_mux);
    	paused = false;
	iter_max = 0;
    }
    pause_cond.notify_one();
}

void Actor::setIteration(int iter) {
	iterno = iter;
}

int Actor::resumeTill(int iter) {

    lock_guard<mutex> lockpause(pause_mux);
    if (iter < iterno) {
	    log("resume_till: actor cannot resume until a smaller iteration.");
	    return -1;
    }

    if (iter == iterno)
    	return 0;

    //while(!paused)
    //	pause_cond.wait(lockpause);
    
    while(iterno < iter) {
	runIter();
    }
    return 0;
}

void Actor::setPeriod(int p) {
	average_period = p;
	instance_period = p;
}

int Actor::getPeriod() {
	return average_period/1000;
}

unsigned long Actor::getLatency() {
	return latency;
}

int Actor::getOutPortOcc(std::string port, int idx) {
	return outputPorts[port]->getOccupancy(idx);

}

int Actor::getInPortOcc(std::string port, int idx) {
	return inputPorts[port]->getOccupancy(idx);
}

bool Actor::isSource() {
	return (inputPorts.empty() 
		//&& getPropBool("env") == false
		)
		//|| getPropBool("src") == true
		;
}

bool Actor::isNonSource() {
	return (!inputPorts.empty() 
		//&& getPropBool("env") == false
		//&& getPropBool("src") == false
		);
}

bool Actor::isEnv() {
	return getPropBool("env");
}

bool Actor::isSink() {
	return outputPorts.empty();
}

void Actor::terminate() {
    if (!logging){
        logfile.close();
    }
}

Actor::~Actor() { 
    terminate();
}
