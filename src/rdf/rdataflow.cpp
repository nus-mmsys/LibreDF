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
	curr_graph = new Graph();
}

void RDataflow::set_graph(RDFGraph * r) {
	rdfg = r;
	curr_graph->reconfigure_from(r->graph);
}

Rule * RDataflow::get_applicable_rule() {
	Rule * r = nullptr;
	//The load is a dummy variable for the tests.
	//In real application, conditions such as latency and throughput can be used. 
	load++;
	for (auto c : rdfg->prog) {
	    for (auto ac : rdfg->prog[c.first]) {
		if (actors.find(c.first) != actors.end()) {
			if (actors[c.first]->getPeriod() == ac.val) {
				actors[c.first]->setPeriod(0); 
				return rdfg->rules[ac.rule];
			}
		} else {
 			if (ac.val == load)
				return rdfg->rules[ac.rule];
		}
	    }
	}
	return r;
}

void RDataflow::reconfigure(int iter) {
	auto g = curr_graph;
	string srcname, snkname;
	int place_init = actors.size();

	//g->print();

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
			//destroyActor(it->second);
			actors.erase(it++);
		} else {
			++it;
		}
	}

	//Create appearing actors.
	string type;
	vector<df::Actor *> appac;
	vector<df::Actor *> repac;
	df::Actor * actmp;
	for (auto c : g->get_actors()) {
		if (actors.find(c) == actors.end()) {
			type = g->get_actor_type(c); 
			actmp = createActor(type,c);
			actmp->setSolution(g->get_solution(c));
			actmp->setProps(g->get_actor_props(c));
			appac.push_back(actmp);
		} else {
			actors[c]->setSolution(g->get_solution(c));
			auto rep = actors[c]->setProps(g->get_actor_props(c));
			if (rep)
				repac.push_back(actors[c]);
		}
	}

	for (auto c : repac) {
		c->startReInit();
	}

	for (auto c : repac) {
		c->waitReInit();
	}

	//Create appearing edges.
	vector<df::Edge *> apped;
	df::Edge * edtmp;
	for (auto e : g->get_edges()) {
		srcname = g->get_source_name(e);
		snkname = g->get_sink_name(e);
		edtmp = containsEdge(srcname, snkname);
	       	if (edtmp == nullptr) {
			edtmp = createEdge(e, srcname, snkname);
			apped.push_back(edtmp);
		}
		
		setPortRates(srcname, snkname, 
			g->get_source_port(e), g->get_sink_port(e),
			g->get_source_rate(e), g->get_sink_rate(e));
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

	placement.place(appac, ROUND_ROBIN, place_init);

  	for (auto c : appac) {
    		c->startRun();
  	}
}

void RDataflow::run() {
 
  int iter;
  
  if (status != DataflowStatus::READY) {
    log("[RDF] Dataflow is not ready to run.");
    return;
  }

  log("[RDF] Running the dataflow...");
  timer.start();

  print();

  placement.place(actors, ROUND_ROBIN, 0);
  /* 
   * The controller starts all actors.
   *
   */
  for (auto f : actors) {
    f.second->startRun();
  }
  
  /* 
   * The controller measures some non-functional metrics.
   * These metrics define when a transformation needs to
   * be applied and they are specified in the main
   * of the RDF program.
   *
   * Here get_applicable_rule is a function which returns 
   * the rules that can be applied. (the current implementation 
   * is only for the test purposes)
   *
   * If an applicable rule exists, the controller 
   * has to notify all the actors to puase  
   * and give their iteration numbers. It then take 
   * the maximum value of all iteration numbers and 
   * ask the the actors to continue until
   * this max value. 
   *
   * The controller then performs the transformation 
   * and asks all the actors to resume.
   *
   */

  Rule * r;
  Graph * res;
  Timer rdftimer;
  vector<string> delays;
  while(!check_eos()) {
  	timer.sleep(200);
   	r = get_applicable_rule();
  	if (r==nullptr) {
  		continue;
  	}
	else {
		log("[RDF] Rule "+r->get_name()+" is applicable.");
	}


	//startTiming();
	iter = pause();
	if (iter<0) {
		log("[RDF] Pause failed.");
		return;
	}		
	log("[RDF] Paused at iteration "+to_string(iter));
	//endTiming("[RDF] Pausing delay: ");

	rdftimer.start();
	
	//startTiming();
	res = r->apply(curr_graph);
	if (res!=nullptr) {
		curr_graph->reconfigure_from(res);
		//endTiming("[RDF] Matching delay: ");
		
		//startTiming();
		reconfigure(iter);
		//endTiming("[RDF] Replace graph delay: ");
	}

	delays.push_back(rdftimer.endUs());
	
	//startTiming();
	resume();
	//endTiming("[RDF] Resuming delay: ");
  }

  status = DataflowStatus::RUNNING;
  
  /*
   * The controller wait for all actors to end.
   *
   */
  for (auto f : actors) {
    f.second->waitRun();
  }
 
  for (auto d : delays)
	 log("RDF reconfiguration delay = "+d+" us");

  log("Execution time = "+timer.endUs()+" us"); 
  
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
