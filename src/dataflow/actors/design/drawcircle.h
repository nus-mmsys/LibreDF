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

#ifndef DF_DRAWCIRCLE_H_
#define DF_DRAWCIRCLE_H_

#include "core/df.h"
#include "tokens/opencv/mat.h"
#include "tokens/basic/float3d.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

class DrawCircle: public df::Actor {
  
private:
  
  cv::Mat img;
  cv::Point center;
 
  df::InputPort<df::Float3D> * input;
  df::OutputPort<df::Mat> * output;
 
  static  df::ActorRegister<DrawCircle> reg;
public:
  
  DrawCircle(const string& name);
  
  virtual void init();
  
  virtual void run();
  
  virtual ~DrawCircle();
  
};

#endif /* DF_DRAWCIRCLE_H_ */
