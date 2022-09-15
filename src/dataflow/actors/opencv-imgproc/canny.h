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

#ifndef DF_CANNY_H_
#define DF_CANNY_H_

#include "core/df.h"
#include "tokens/opencv/mat.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>

using namespace std;

class Canny: public df::Actor {
  
private:

  cv::Mat frame;
  int threshold, ratio, kernel_size;
  df::InputPort<df::Mat> * input;
  df::OutputPort<df::Mat> * output;
 
  int cvt;
  int delay;

  static  df::ActorRegister<Canny> reg;
public:
  
  Canny(const string& name);
  
  virtual void init();
  
  virtual void run();
  
  virtual ~Canny();
  
};

#endif /* DF_CANNY_H_ */
