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

#include "dct.h"

using namespace df;
using namespace std;

ActorRegister<Dct> Dct::reg("Dct");

Dct::Dct(const string& name) : Actor(name) {
  input = createInputPort<df::Mat>("input");
  output = createOutputPort<df::Mat>("output");
}

void Dct::init() {

}

void Dct::run() {

  auto in = consume(input);	
  auto out = produce(output);
  in->get()->convertTo(*out->get(), CV_32F);
  cv::dct(*out->get(), *out->get());
  log("sending frame "+to_string(stepno));
  release(input);
  release(output);

}

Dct::~Dct() {
  destroyPort(input);
  destroyPort(output);
}
