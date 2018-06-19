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

#include "gemm.h"

using namespace df;
using namespace std;

ActorRegister<Gemm> Gemm::reg("Gemm");

Gemm::Gemm(const string& name) : Actor(name) {
  input1 = createInputPort<cv::Mat>("input1");
  input2 = createInputPort<cv::Mat>("input2");
  output = createOutputPort<cv::Mat>("output");
}

void Gemm::init() {
}

void Gemm::run() {

  cv::Mat * in1 = consume(input1);	
  cv::Mat * in2 = consume(input2);

  in1->convertTo(*in1,CV_32F);
  in2->convertTo(*in2,CV_32F);

  cv::split(*in1, in1planes);
  cv::split(*in2, in2planes);

  release(input1);
  release(input2);

  cv::gemm(in1planes[0],in2planes[0], 1.0/in1planes[0].cols, cv::Mat(), 0.0, outplanes[0]);
  cv::gemm(in1planes[1],in2planes[1], 1.0/in1planes[1].cols, cv::Mat(), 0.0, outplanes[1]);
  cv::gemm(in1planes[2],in2planes[2], 1.0/in1planes[2].cols, cv::Mat(), 0.0, outplanes[2]);

  cv::sqrt(outplanes[0], outplanes[0]);
  cv::sqrt(outplanes[1], outplanes[1]);
  cv::sqrt(outplanes[2], outplanes[2]);

  cv::Mat * out = produce(output);
  cv::merge(outplanes, 3, *out);
  out->convertTo(*out,CV_8U);
  log("sending "+to_string(stepno));
  release(output);

}

Gemm::~Gemm() {
  destroyPort(input1);
  destroyPort(input2);
  destroyPort(output);
}
