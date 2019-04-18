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

#include "matmerge.h"

using namespace df;
using namespace std;

ActorRegister<MatMerge> MatMerge::reg("MatMerge");

MatMerge::MatMerge(const string& name) : Actor(name) {
  input = createInputPortVector<df::Mat>("input");
  output = createOutputPort<df::Mat>("output");
}

void MatMerge::init() {

  if (propEmpty("level"))
	  level = 2;
  else
	  level = getPropInt("level");

  tilew = 0;
  tileh = 0;

  input->setArity(level * level);

}

void MatMerge::reinit() {

  auto newlevel = getPropInt("level");

  if (newlevel > level) {
  	input->addArity((newlevel*newlevel) - (level*level));
  }

  if (newlevel < level) {
	//TODO
	//input->removeArity((level*level) - (newlevel*newlevel));
  }

  level = newlevel;
  tilew = 0;
  tileh = 0;

}

void MatMerge::run() {

  auto in = consume(input);	
  auto out = produce(output);
 
  if (tilew == 0 || tileh == 0) { 
  	tilew = in[0]->get()->cols;
  	tileh = in[0]->get()->rows;
	type = in[0]->get()->type();
	size = in[0]->get()->total()*in[0]->get()->elemSize();
	out->mat_init(tilew*level, tileh*level, type, size*level*level);
  	for (int i=0; i<output->getBufferSize();i++)
		output->get(i)->mat_init(tilew*level, tileh*level, type, size*level*level);
  }

  for (int j=0; j < level ; j++) {
  	for (int i=0; i < level ; i++) {
		cv::Rect tile(i * tilew,
				j * tileh,
				tilew, tileh);

		out->set_view(tile, in[j*level+i]->get());
	}
  }

  log("sending "+to_string(stepno));
  release(input);
  release(output);

}

MatMerge::~MatMerge() {
  destroyPort(input);
  destroyPort(output);
}
