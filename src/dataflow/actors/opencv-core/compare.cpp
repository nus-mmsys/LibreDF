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

#include "compare.h"

using namespace df;
using namespace std;

ActorRegister<Compare> Compare::reg("Compare");

Compare::Compare(const string& name) : Actor(name) {
  input1 = createInputPort<cv::Mat>("input1");
  input2 = createInputPort<cv::Mat>("input2");
  output = createOutputPort<cv::Mat>("output");
}

void Compare::init() {

 
  if (!propEmpty("operation")) {
	  string op = getProp("operation");
	  if (op == "eq")
		  operation = cv::CMP_EQ;
	  else if (op == "gt")
		  operation = cv::CMP_GT;
    	  else if (op == "ge")
		  operation = cv::CMP_GE;
	  else if (op == "lt")
		  operation = cv::CMP_LT;
	  else if (op == "le")
		  operation = cv::CMP_LE;
	  else if (op == "ne")
		  operation = cv::CMP_NE;

  } else
	  operation = cv::CMP_EQ;


}

void Compare::run() {

  cv::Mat * in1 = consume(input1);	
  cv::Mat * in2 = consume(input2);	
  cv::Mat * out = produce(output);
  cv::compare(*in1, *in2, *out, operation);
  log("sending "+to_string(stepno));
  release(input1);
  release(input2);
  release(output);

}

Compare::~Compare() {
  destroyPort(input1);
  destroyPort(input2);
  destroyPort(output);
}
