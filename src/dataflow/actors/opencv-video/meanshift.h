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

#ifndef DF_MEANSHIFT_H_
#define DF_MEANSHIFT_H_

#include "core/df.h"
#include "tokens/opencv/mat.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video/video.hpp>

#include <iostream>
#include <string>

using namespace std;

class MeanShift: public df::Actor {
  
private:

  cv::Mat frame;
  cv::Mat gray;
  float * phranges[3];
  cv::Mat roi_hist, hsv, dst;
  int ch[3];
  cv::Rect track_window;
  std::string file_name;

  df::InputPort<df::Mat> * input;
 
  static  df::ActorRegister<MeanShift> reg;
public:
  
  MeanShift(const string& name);
  
  virtual void init();
  
  virtual void run();
  
  virtual ~MeanShift();
  
};

#endif /* DF_MEANSHIFT_H_ */
