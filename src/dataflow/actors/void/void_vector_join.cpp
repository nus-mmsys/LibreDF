/*
 *   TMF
 *   Copyright (C) TMF Team
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

#include "void_vector_join.h"

using namespace df;
using namespace std;

ActorRegister<VoidVectorJoin> VoidVectorJoin::reg("VoidVectorJoin");

VoidVectorJoin::VoidVectorJoin(const string& name) : Actor(name) {
  input = createInputPortVector<df::IntVector>("input");
  output = createOutputPort<df::IntVector>("output");
}

void VoidVectorJoin::init() {
  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");
}

void VoidVectorJoin::reinit() {

  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");
}

void VoidVectorJoin::run() {

  auto in = consume(input);	
  auto out = produceMR(output);

  start_exec = timer.nowUs();
  
  for (int j=0; j<out.size(); j++) {
	  out[j]->clear_items();
	  for (int k=0; k<in[j]->vector_size(); k++) {
		  out[j]->put_item(in[j]->get_item(k));
	  }
  }

  log("void join "+to_string(stepno));

  end_exec = timer.nowUs();
  
  release(input);
  releaseMR(output);

}

VoidVectorJoin::~VoidVectorJoin() {
  destroyPort(input);
  destroyPort(output);
}
