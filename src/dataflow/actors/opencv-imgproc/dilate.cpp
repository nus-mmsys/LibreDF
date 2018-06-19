/*
 *   DF
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

#include "dilate.h"

using namespace df;
using namespace std;

ActorRegister<Dilate> Dilate::reg("Dilate");

Dilate::Dilate(const string& name) : Actor(name) {
  input = createInputPort<cv::Mat>("input");
  output = createOutputPort<cv::Mat>("output");
}

void Dilate::init() {

  if (!propEmpty("dilation_size"))
	 dilation_size = getPropInt("dilation_size");
  else
	 dilation_size = 1;

  kernel = cv::getStructuringElement( cv::MORPH_RECT,
                       cv::Size( 2 * dilation_size + 1, 2 * dilation_size+1 ),
                       cv::Point( dilation_size, dilation_size ) );
}

void Dilate::run() {

  cv::Mat * in = consume(input);	
  cv::Mat * out = produce(output);
  cv::dilate(*in, *out, kernel);
  log("sending frame "+to_string(stepno));
  release(input);
  release(output);

}

Dilate::~Dilate() {
  destroyPort(input);
  destroyPort(output);
}
