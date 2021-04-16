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

#include "delay_vector_source.h"

using namespace df;
using namespace std;

ActorRegister<DelayVectorSource> DelayVectorSource::reg("DelayVectorSource");

DelayVectorSource::DelayVectorSource(const std::string & name): df::Actor(name) {
  output = createOutputPort<df::IntVector>("output");
}

void DelayVectorSource::init() {
  if (!propEmpty("delay"))
    delay = getPropInt("delay");
  else
    delay = 1;

  if (!propEmpty("last"))  	
  	last = getPropInt("last");
  else
        last = 1000;

  if (!propEmpty("volume"))  	
  	volume = getPropInt("volume");
  else
        volume = 1000000;


  max = delay * 720000;
}

void DelayVectorSource::reinit() {
}

void DelayVectorSource::run() {
  auto out = produce(output);

  start_exec = timer.nowUs();

  out->clear_items();
  for (int k=0; k<volume; k++)
      out->put_item(stepno);
  
  log("delay source "+to_string(stepno));

  counter = 0;
  while(counter<max) counter++;

  if(stepno >= last)
    setEos(output);

  end_exec = timer.nowUs();

  release(output);
}

DelayVectorSource::~DelayVectorSource() {
  destroyPort(output);
}

