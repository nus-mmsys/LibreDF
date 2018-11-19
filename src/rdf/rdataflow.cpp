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

void RDataflow::run() {
  
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
   * The controller measures some criteria
   * These criteria are specified in the main
   * of the RDF program.
   *
   * For a test, we can have a function throuput()
   * which generates a random value. Based on the
   * value, the controller decides whether to perform 
   * the transformation or not.
   *
   * If it decides to perform the transformation,
   * it has to notify all the actors to stop at 
   * a given iteration. To know which iteration they 
   * have to stop at, the controller asks the source
   * to give its iteration number and stop.
   *
   * This number is given to all actors and actors eventually 
   * stop at the given iteration. after getting the acknowledgement 
   * from all actors, the controller knows that it can perform 
   * the transformation. It performs the transformation and asks
   * the sources to continue.
   *
   * This loop continues untill the program comes to an end, 
   * and the contoller asks every actor to stop.
   *
   */

  //while(!check_eos()) {

	/*
   	Rule * r = get_applicable_rule();
  	if (r==nullptr) {
  		sleep(100);
  		continue;
  	}

	start = std::chrono::high_resolution_clock::now(); 
	*/

  //-	vector<Actor *> sources = find_sources();
  //-	for (auto s : sources) {
  //-		ask_to_stop(sources);
  //-	}
  //-	vector<Actor *> sinks = find_sinks();
  //-	for (auto s : sinks) {
  //-		wait_for_ack(s);
  //-	}
  //-
  //-	apply(r);
  //-
  //-	vector<Actor *> sources = find_sources();
  //-	for (auto s : sources) {
  //-		ask_to_continue(sources);
  //-	}

  	/*
	end = std::chrono::high_resolution_clock::now(); 
	cout << "Reconfiguration delay: "
		<< std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) 
		<< "\n";
	*/

  //}

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


