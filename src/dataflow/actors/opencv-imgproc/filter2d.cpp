/*
 *   LibreDF
 *   Copyright (C) A. Shafiei
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

#include "filter2d.h"

using namespace df;
using namespace std;

ActorRegister<Filter2D> Filter2D::reg("Filter2D");

Filter2D::Filter2D(const string& name) : Actor(name) {
  input = createInputPort<df::Mat>("input");
  output = createOutputPort<df::Mat>("output");
}

void Filter2D::init() {

  if (getProp("kernel_size") != "")
    kernel_size = getPropInt("kernel_size");
  else
    kernel_size = 3;
  anchor = cv::Point(-1,-1);
  delta = 0;
  ddepth = -1;

  kernel = cv::Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);

}

void Filter2D::run() {

  auto in = consume(input);	
  auto out = produce(output);
  cv::filter2D(*in->get(), *out->get(), ddepth, kernel, anchor, delta, cv::BORDER_DEFAULT);
  log("sending frame "+to_string(stepno));
  release(input);
  release(output);

}

Filter2D::~Filter2D() {
  destroyPort(input);
  destroyPort(output);
}
