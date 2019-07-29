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

  if (propEmpty("text"))
	  text = true;
  else
	  text = getPropBool("text");

  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");

  tilew = 0;
  tileh = 0;

  nh = arith.factor(arity);
  nw = arity/nh;
 
}

void MatMerge::reinit() {

  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");

  tilew = 0;
  tileh = 0;

  nh = arith.factor(arity);
  nw = arity/nh;
 
}

void MatMerge::run() {

  auto in = consume(input);	
  auto out = produce(output);
 
  if (tilew == 0 || tileh == 0) { 
  	tilew = in[0]->get()->cols;
  	tileh = in[0]->get()->rows;
	type = in[0]->get()->type();
	size = in[0]->get()->total()*in[0]->get()->elemSize();
	out->mat_init(tilew*nw, tileh*nh, type, size*arity);
  	for (int i=0; i<output->getBufferSize();i++)
		output->get(i)->mat_init(tilew*nw, tileh*nh, type, size*arity);
  }

  for (int j=0; j < nh ; j++) {
  	for (int i=0; i < nw ; i++) {
		cv::Rect tile(i * tilew,
				j * tileh,
				tilew, tileh);
		out->set_view(tile, in[j*nh+i]->get());
		if (text)
 			cv::putText(*out->get(), to_string(i+j*nh), cv::Point(i*tilew+tilew/2, j*tileh+tileh/2), cv::FONT_HERSHEY_DUPLEX, 3, cv::Scalar(255,0,0), 2, 8, false);
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
