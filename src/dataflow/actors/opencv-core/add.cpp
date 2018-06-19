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

#include "add.h"

using namespace df;
using namespace std;

ActorRegister<Add> Add::reg("Add");

Add::Add(const string& name) : Actor(name) {
  input1 = createInputPort<cv::Mat>("input1");
  input2 = createInputPort<cv::Mat>("input2");
  output = createOutputPort<cv::Mat>("output");
}

void Add::init() {


}

void Add::run() {

  cv::Mat * in1 = consume(input1);	
  cv::Mat * in2 = consume(input2);	
  cv::Mat * out = produce(output);
  *out = (*in1) + (*in2);
  log("sending "+to_string(stepno));
  release(input1);
  release(input2);
  release(output);

}

Add::~Add() {
  destroyPort(input1);
  destroyPort(input2);
  destroyPort(output);
}
