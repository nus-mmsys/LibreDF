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

#include "cvtcolor.h"

using namespace df;
using namespace std;

ActorRegister<CvtColor> CvtColor::reg("CvtColor");

CvtColor::CvtColor(const string& name) : Actor(name) {
  input = createInputPort<df::Mat>("input");
  output = createOutputPort<df::Mat>("output");
}

void CvtColor::init() {
  if (!propEmpty("cvt")) {
	string cvtstr = getProp("cvt");
	if (cvtstr == "bgr2gray")
		cvt = CV_BGR2GRAY;
	if (cvtstr == "gray2bgr")
		cvt = CV_GRAY2BGR;
  } else {
	  cvt = CV_BGR2GRAY;
  }
}

void CvtColor::run() {

  auto in = consume(input);
  auto out = produce(output);
  cv::cvtColor(*in->get(), *out->get(), cvt);
  log("sending frame "+to_string(stepno));
  release(input);
  release(output);

}

CvtColor::~CvtColor() {
  destroyPort(input);
  destroyPort(output);
}
