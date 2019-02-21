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
	load = 0;
}

void RDataflow::set_graph(RDFGraph * r) {
	rdfg = r;
}

void RDataflow::log(string msg) {
	iolock.lock();
	cout << msg << endl;
	iolock.unlock();
}

Rule * RDataflow::get_applicable_rule() {
	Rule * r = nullptr;
	//The load is a dummy variable for the tests.
	//In real application, conditions such as latency and throughput can be used. 
	load++;
	for (auto c : rdfg->prog["load"]) {
		if (c.val==load)
			return rdfg->rules[c.rule];
	}
	return r;
}

void RDataflow::reconfigure(int iter) {
	auto g = rdfg->graph;
	string srcname, snkname;

	/*
	for (auto c : g->get_actors()) {
		cout << c << " : " << g->get_actor_type(c) << "\n";
	}

	for (auto e : g->get_edges()) {
		cout << e << " : " << g->get_source_name(e)
		       << " -> " << g->get_sink_name(e)	<< "\n";
	}
	*/


	//Remove dissappearing edges.
	for (auto it = edges.cbegin(); it != edges.cend();)
	{
		srcname = it->second->getSource()->getName();
	        snkname = it->second->getSink()->getName();
		if (!g->contains_edge(srcname, snkname)) {
			disconnectActors(it->second->getSource(),
					   it->second->getSink(),
					   it->first);
			
			destroyEdge(it->second);
			edges.erase(it++);
  		} else {
			++it;
  		}
	}
		
	//Remove dissappearing actors.
	for (auto c : actors) {
	for (auto it = actors.cbegin(); it != actors.cend();)
		if (!g->contains_actor(it->first)) {
			destroyActor(it->second);
			actors.erase(it++);
		} else {
			++it;
		}
	}

	//Create appearing actors.
	string type;
	vector<df::Actor *> appac;
	for (auto c : g->get_actors()) {
		if (actors.find(c) == actors.end()) {
			type = g->get_actor_type(c); 
			appac.push_back(createActor(type,c));
		}
	}

	//Create appearing edges.
	vector<df::Edge *> apped;
	for (auto e : g->get_edges()) {
		srcname = g->get_source_name(e);
		snkname = g->get_sink_name(e);
		if (!containsEdge(srcname, snkname)) {
			apped.push_back(createEdge(e, srcname, snkname));
		}
	}	
	
	print();

	for (auto c : appac) {
		setDataflowProp(c);
		c->startInit();
	}	

	for (auto c : appac) {
		c->waitInit();
		c->setIteration(iter);
	}	

	for (auto & ed : apped) {
		connectActors(ed->getSource(), 
			      ed->getSink(), 
			      ed->getName(), 
			      ed->getSourceRate(), 
			      ed->getSinkRate());
	}

	int cpunb = std::thread::hardware_concurrency();
  	int cpuid = 0; 

  	for (auto c : appac) {
    		c->startRun(cpuid);
    		cpuid = (cpuid + 1) % cpunb;
  	}
	
}

void RDataflow::run() {
 
  int iter;

  if (status != DataflowStatus::READY) {
    log("[RDF] Dataflow is not ready to run.");
    return;
  }

  int cpunb = std::thread::hardware_concurrency();
  int cpuid = 0; 

  log("[RDF] Running the dataflow...");
  start = std::chrono::high_resolution_clock::now();

  print();

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

  Rule * r;
  Graph * res;
  while(!check_eos()) {
	
   	r = get_applicable_rule();
  	if (r==nullptr) {
  		sleep(500);
  		continue;
  	}
	else {
		log("[RDF] Rule "+r->get_name()+" is applicable.");
	}
  
	start = std::chrono::high_resolution_clock::now(); 

	iter = pause();
	if (iter<0) {
		log("[RDF] Pause failed.");
		return;
	}		
	log("[RDF] Paused at iteration "+to_string(iter));

	res = r->apply(rdfg->graph);
	if (res!=nullptr)
		rdfg->graph = res;

	reconfigure(iter);

	resume();

	end = std::chrono::high_resolution_clock::now(); 
	log("[RDF] Reconfiguration delay: "+
		std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count())+" ms");
  }
  

  status = DataflowStatus::RUNNING;
  
  /*
   * The controller wait for all actors to end.
   *
   */
  for (auto f : actors) {
    f.second->waitRun();
  }
  
  end = std::chrono::high_resolution_clock::now();
  log("Execution time = "+to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count())+" ms"); 
  
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


