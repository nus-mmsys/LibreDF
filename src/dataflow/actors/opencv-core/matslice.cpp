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

#include "matslice.h"

using namespace df;
using namespace std;

ActorRegister<MatSlice> MatSlice::reg("MatSlice");

MatSlice::MatSlice(const string& name) : Actor(name) {
  input = createInputPort<df::Mat>("input");
  output = createOutputPortVector<df::Mat>("output");
}

void MatSlice::init() {

  if (propEmpty("level"))
	  level = 2;
  else
	  level = getPropInt("level");

  tilew = 0;
  tileh = 0;
  
  nh = arith.factor(level);
  nw = level/nh;
  
  output->setArity(level);

}

void MatSlice::reinit() {

  auto newlevel = getPropInt("level");
  
  if (newlevel > level) {
  	output->addArity(newlevel - level);
  }

  level = newlevel;
  tilew = 0;
  tileh = 0;

  nh = arith.factor(level);
  nw = level/nh;
 
}

void MatSlice::run() {

  auto in = consume(input);	
  auto out = produce(output);
 
  if (tilew == 0 || tileh == 0) { 
  	tilew = in->get()->cols / nw;
  	tileh = in->get()->rows / nh;
  }

  for (int j=0; j < nh ; j++) {
	  for (int i=0; i < nw ; i++) {
	  	cv::Rect tile(i * tilew,
				j * tileh,
				tilew, tileh);
  	  	out[j*nh+i]->set(in->crop(tile));
	  }
  }

  log("sending "+to_string(stepno));
  release(input);
  release(output);

}

MatSlice::~MatSlice() {
  destroyPort(input);
  destroyPort(output);
}
