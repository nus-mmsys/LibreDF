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

#include "convolution.h"

using namespace df;
using namespace std;

ActorRegister<Convolution> Convolution::reg("Convolution");

Convolution::Convolution(const string& name) : Actor(name) {
  input = createInputPort<df::Mat>("input");
  output = createOutputPort<df::Mat>("output");
}

void Convolution::init() {

  if (!propEmpty("kernel_size"))
    kernel_size = getPropInt("kernel_size");
  else
    kernel_size = 3;

  kernel = cv::Mat::ones( kernel_size, kernel_size, CV_32F ) /
	   (float)(kernel_size*kernel_size);
 
  if (!propEmpty("kernel_val")) {
    int kernel_val = getPropInt("kernel_val");
    for (int i=0; i<kernel_size; i++)
    	for (int j=0; j<kernel_size; j++)
		kernel.at<cv::Vec2f>(j,i)[0] = kernel_val /
			(float)(kernel_size*kernel_size);
  }

  anchor = cv::Point(0,0);
  delta = 0;
  ddepth = -1;

  /* Filter2D computes correlation.
   * To get a convolution, the kernel 
   * needs to be flipped and the anchor
   * needs to be set.
   */
  cv::flip(kernel, kernel, 1);
}

void Convolution::run() {

  anchor = cv::Point(kernel.cols - anchor.x - 1, kernel.rows - anchor.y -1);
  df::Mat * in = consume(input);	
  df::Mat * out = produce(output);
  cv::filter2D(*in->get(), *out->get(), ddepth, kernel, anchor, delta, cv::BORDER_DEFAULT);
  log("sending frame "+to_string(stepno));
  release(input);
  release(output);

}

Convolution::~Convolution() {
  destroyPort(input);
  destroyPort(output);
}
