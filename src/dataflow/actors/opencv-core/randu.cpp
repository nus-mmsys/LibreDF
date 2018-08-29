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

#include "randu.h"

using namespace df;
using namespace std;

ActorRegister<Randu> Randu::reg("Randu");

Randu::Randu(const string& name) : Actor(name) {
  output = createOutputPort<df::Mat>("output");
}

void Randu::init() {

  if (!propEmpty("width"))
    width = getPropInt("width");
  else
    width = 162;

  if (!propEmpty("height"))
    height = getPropInt("height");
  else
    height = 100;

  if (!propEmpty("last"))
    last = getPropInt("last");
  else
    last = 100;

  frame = new cv::Mat(height, width, CV_8UC3, cv::Scalar(0,0,0)); 
}

void Randu::run() {


  cv::randu(*frame, cv::Scalar(0,0,0), cv::Scalar(256,256,256));
  auto out = produce(output);
  out->set(*frame);
  log("sending "+to_string(stepno));
  sleep(100);

  if (stepno == last)
    setEos(output);

  release(output);
}

Randu::~Randu() {
  destroyPort(output);
}
