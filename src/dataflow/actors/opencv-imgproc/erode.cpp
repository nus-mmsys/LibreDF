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

#include "erode.h"

using namespace df;
using namespace std;

ActorRegister<Erode> Erode::reg("Erode");

Erode::Erode(const string& name) : Actor(name) {
  input = createInputPort<cv::Mat>("input");
  output = createOutputPort<cv::Mat>("output");
}

void Erode::init() {

  if (!propEmpty("erosion_size"))
	  erosion_size = getPropInt("erosion_size");
  else
	  erosion_size = 0;

  kernel = getStructuringElement( cv::MORPH_RECT,
                       cv::Size( 2 * erosion_size + 1, 2 * erosion_size + 1 ),
                       cv::Point( erosion_size, erosion_size ) );
}

void Erode::run() {

  cv::Mat * in = consume(input);	
  cv::Mat * out = produce(output);
  cv::erode(*in, *out, kernel);
  log("sending frame "+to_string(stepno));
  release(input);
  release(output);

}

Erode::~Erode() {
  destroyPort(input);
  destroyPort(output);
}
