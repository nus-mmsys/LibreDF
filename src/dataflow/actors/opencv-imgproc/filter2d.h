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

#ifndef DF_FILTER2D_H_
#define DF_FILTER2D_H_

#include "core/df.h"
#include "tokens/opencv/mat.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>

using namespace std;

class Filter2D: public df::Actor {
  
private:

  cv::Mat frame, kernel;
  int kernel_size, delta, ddepth;
  cv::Point anchor;
  df::InputPort<df::Mat> * input;
  df::OutputPort<df::Mat> * output;
 
  static  df::ActorRegister<Filter2D> reg;
public:
  
  Filter2D(const string& name);
  
  virtual void init();
  
  virtual void run();
  
  virtual ~Filter2D();
  
};

#endif /* DF_FILTER2D_H_ */
