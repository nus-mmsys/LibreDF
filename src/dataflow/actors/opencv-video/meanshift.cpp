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

#include "meanshift.h"

using namespace df;
using namespace std;

ActorRegister<MeanShift> MeanShift::reg("MeanShift");

MeanShift::MeanShift(const string& name) : Actor(name) {
  input = createInputPort<cv::Mat>("input");
}

void MeanShift::init() {

  int hsize[] = { 16,16,16 };
  float hranges[] = { 0,180 };
  phranges[0] = hranges;
  phranges[1] = hranges;
  phranges[2] = hranges;

  cv::Rect r(10, 10, 100, 100);
  track_window = r;

  //What we need to track needs to be specified.
  cv::Mat * in = consume(input);
  frame = in->clone();  
  release(input);

  cv::Mat roi = frame(r);
  cv::Mat hsv_roi;
  cv::cvtColor(roi, hsv_roi, cv::COLOR_BGR2HSV); 
  cv::Mat maskroi;
  cv::inRange(hsv_roi, cv::Scalar(0.0, 60.0, 32.0), cv::Scalar(180.0, 255.0, 255.0),maskroi); 
  cv::Mat roi_hist;
  ch[0] = 0;
  ch[1] = 1;
  ch[2] = 2;
  cv::calcHist(&roi, 1, ch, maskroi, roi_hist, 1, hsize, const_cast<const float**>(phranges));
  cv::normalize(roi_hist, roi_hist, 0, 255, cv::NORM_MINMAX);
}

void MeanShift::run() {

  cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV); 
  cv::calcBackProject(&hsv, 1, ch, roi_hist, dst, const_cast<const float**>(phranges));

  cv::meanShift(dst, track_window, cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 10, 1));

  log("meanShift rect in "+to_string(stepno)+" ("+to_string(track_window.x)+", "+to_string(track_window.y)+", "+to_string(track_window.width)+", "+to_string(track_window.height)+")");
  
  cv::rectangle(frame, track_window, cv::Scalar(0, 255, 0), 2);
  
  file_name = dfout_path + std::to_string(stepno) + ".png";
  cv::imwrite(file_name, frame); 
 
  cv::Mat * in = consume(input);
  frame = in->clone();  
  release(input);

}

MeanShift::~MeanShift() {
  destroyPort(input);
}
