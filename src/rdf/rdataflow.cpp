/*
 *   RDF
 *   Copyright (C) INRIA - Orange Labs
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

#include "rdataflow.h"

using namespace df;
using namespace std;

RDataflow::RDataflow(string name) : df::Dataflow(name) {

}

Rule * RDataflow::get_applicable_rule() {
	Rule * r = nullptr;
	//TODO
	return r;
}

void RDataflow::apply(Rule * r) {
	//TODO
}

void RDataflow::run() {
 
  int iter;

  if (status != DataflowStatus::READY) {
    std::cout << "RDF is not ready to run." << endl;
    return;
  }

  int cpunb = std::thread::hardware_concurrency();
  int cpuid = 0; 

  std::cout << "Running the RDF...\n";
  start = std::chrono::high_resolution_clock::now();

  /* 
   * The controller starts all actors.
   *
   */
  for (auto f : actors) {
    f.second->startRun(cpuid);
    cpuid = (cpuid + 1) % cpunb;
  }

  
  /* 
   * The controller measures some non-functional metrics.
   * These metrics define when a transformation needs to
   * be applied and they are specified in the main
   * of the RDF program.
   *
   * For a test, we can use a dummy function 'metric()'
   * which generates a random value. Based on the
   * value, the controller decides whether to perform 
   * the transformation or not.
   *
   * If it decides to perform the transformation,
   * it has to notify all the source actors to stop  
   * and give their iteration numbers. It then take 
   * the maximum value of all iteration numbers and 
   * ask the source actors to continue until
   * this max value. 
   *
   * This iteration number is sent to sink actors 
   * and sink actors eventually stop at the given iteration
   * and send an acknowledgement to the controller. 
   * The controller then knows that it can perform 
   * the transformation. It performs the transformation 
   * and asks all the sources to continue.
   *
   * This loop continues untill at least one actor
   * reaches the end of stream.
   *
   */

  /*  
  while(!check_eos()) {
	
   	Rule * r = get_applicable_rule();
  	if (r==nullptr) {
  		sleep(100);
  		continue;
  	}

	start = std::chrono::high_resolution_clock::now(); 
	
  	vector<df::Actor *> sinks = find_sinks();
  
	iter = pause();
	if (iter<0) {
		cout << "Pause failed.\n";
		return;
	}		

  	//TODO
 	//	for (auto s : sinks) {
  	//		s->wait_for_ack(iter);
  	//	}
  	
	apply(r);
  
   	resume();

	end = std::chrono::high_resolution_clock::now(); 
	cout << "Reconfiguration delay: "
		<< std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) 
		<< "\n";
  }
  */

  status = DataflowStatus::RUNNING;
  
  /*
   * The controller wait for all actors to end.
   *
   */
  for (auto f : actors) {
    f.second->waitRun();
  }
  
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Execution time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << " ms\n"; 
  
  /*
  for (auto f : actors) {
	std::cout << f.second->getName() << " = " << f.second->getElapsed() << " ms\n";
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


