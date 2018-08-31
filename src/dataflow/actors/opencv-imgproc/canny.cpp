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

#include "canny.h"

using namespace df;
using namespace std;

ActorRegister<Canny> Canny::reg("Canny");

Canny::Canny(const string& name) : Actor(name) {
  input = createInputPort<df::Mat>("input");
  output = createOutputPort<df::Mat>("output");
}

void Canny::init() {

  if (!propEmpty("threshold"))
    threshold = getPropInt("threshold");
  else
    threshold = 100;

  if (!propEmpty("ratio"))
    ratio = getPropInt("ratio");
  else
    ratio = 2;

  if (!propEmpty("kernel_size"))
    kernel_size = getPropInt("kernel_size");
  else
    kernel_size = 3;

}

void Canny::run() {

  auto in = consume(input);
  auto out = produce(output);
  cv::blur(*in->get(), *out->get(), cv::Size(3,3));  
  cv::Canny(*out->get(), *out->get(), threshold, threshold*ratio, kernel_size);
  log("sending frame "+to_string(stepno));
  release(input);
  release(output);

}

Canny::~Canny() {
  destroyPort(input);
  destroyPort(output);
}
