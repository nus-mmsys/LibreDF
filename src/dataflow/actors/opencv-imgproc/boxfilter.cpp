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

#include "boxfilter.h"

using namespace df;
using namespace std;

ActorRegister<BoxFilter> BoxFilter::reg("BoxFilter");

BoxFilter::BoxFilter(const string& name) : Actor(name) {
  input = createInputPort<cv::Mat>("input");
  output = createOutputPort<cv::Mat>("output");
}

void BoxFilter::init() {

  if (getProp("kernel_size") != "")
    kernel_size = getPropInt("kernel_size");
  else
    kernel_size = 7;

  if (getProp("ddepth") != "")
    ddepth = getPropInt("ddepth");
  else
    ddepth = 0;

  anchor = cv::Point(-1,-1);
  ksize = cv::Size(kernel_size, kernel_size);

}

void BoxFilter::run() {

  cv::Mat * in = consume(input);	
  cv::Mat * out = produce(output);
  cv::boxFilter(*in, *out, ddepth, ksize, anchor, true, cv::BORDER_DEFAULT);
  log("sending frame "+to_string(stepno));
  release(input);
  release(output);

}

BoxFilter::~BoxFilter() {
  destroyPort(input);
  destroyPort(output);
}
