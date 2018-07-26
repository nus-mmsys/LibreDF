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

#include "matsplit.h"

using namespace df;
using namespace std;

ActorRegister<MatSplit> MatSplit::reg("MatSplit");

MatSplit::MatSplit(const string& name) : Actor(name) {
  input = createInputPort<df::Mat>("input");
  output = createOutputPortVector<df::Mat>("output");
}

void MatSplit::init() {

  if (propEmpty("level"))
	  level = 2;
  else
	  level = getPropInt("level");

  tilew = 0;
  tileh = 0;

  output->setArity(level * level);

}

void MatSplit::run() {

  auto in = consume(input);	
  auto out = produce(output);
 
  if (tilew == 0 || tileh == 0) { 
  	tilew = in->get()->cols / level;
  	tileh = in->get()->rows / level;
  }

  for (int i=0; i < level*level ; i++) {
	  cv::Rect tile(level * tilew,
			level * tileh,
			tilew, tileh);
  	  //in->get()(tile).copyTo(out->get(i));
  }

  log("sending "+to_string(stepno));
  release(input);
  //release(output);

}

MatSplit::~MatSplit() {
  destroyPort(input);
  //destroyPort(output);
}
