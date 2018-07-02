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

#ifndef DF_MULTIPLY_H_
#define DF_MULTIPLY_H_

#include "core/df.h"
#include "types/opencv/mat.h"
#include <opencv2/core/core.hpp>

#include <iostream>
#include <string>

using namespace std;

class Multiply: public df::Actor {
  
private:

  cv::Mat frame;
  double scale;
  df::InputPort<df::Mat> * input1;
  df::InputPort<df::Mat> * input2;
  df::OutputPort<df::Mat> * output;
 
  static  df::ActorRegister<Multiply> reg;
public:
  
  Multiply(const string& name);
  
  virtual void init();
  
  virtual void run();
  
  virtual ~Multiply();
  
};

#endif /* DF_MULTIPLY_H_ */
