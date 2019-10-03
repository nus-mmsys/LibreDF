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

#include "video_capture_mr.h"

using namespace df;
using namespace std;

ActorRegister<VideoCaptureMR> VideoCaptureMR::reg("VideoCaptureMR");

VideoCaptureMR::VideoCaptureMR(const string& name) : Actor(name){
  outputMat = createOutputPort<df::Mat>("output");
  cap = nullptr;
}

void VideoCaptureMR::init() {

  if (!propEmpty("var_period"))  	
  	var_period = getPropBool("var_period");
  else
        var_period = false;
 
  if (!propEmpty("period"))  	
  	period = getPropInt("period");
  else
        period = 10;
  
  if (getProp("file_name") != "")  	
  	file_name = fsys.inPath() + "/" + getProp("file_name");
  else
        log("error: file_name is not specified.");
 
  sleep_time = 5*period;
  increase = false;
 
  cap = new cv::VideoCapture(file_name);

  if(!cap->isOpened()){
    cout << "Error opening video stream or file" << endl;
  }
  *cap >> frame;
  padframe = cv::Mat(frame.rows, frame.cols, frame.type());
}

void VideoCaptureMR::run() {

  auto out = produceMR(outputMat);	
  for (int j=0 ; j<out.size(); j++) {

      if(frame.empty()) {
	  while (j<out.size()) {
		  out[j]->set(padframe);
		  j++;
	  }
	  setEos(outputMat);
	  break;
      }

      out[j]->set(frame);
      *cap >> frame;
	  
      if(frame.empty()) {
	  while (j<out.size()) {
		  out[j]->set(padframe);
		  j++;
	  }
	  setEos(outputMat);
	  break;
      }

  }

  log("capturing frame "+to_string(stepno));
  
  releaseMR(outputMat); 
 
  if (var_period)
        timer.sleep(sleep_time/5);
  else 
  	timer.sleep(period);

  if (sleep_time == 5*period)
	 increase = false;
  if (sleep_time == 4.5*period)
	 increase = true;

  if (increase) sleep_time++; else sleep_time--;

}

VideoCaptureMR::~VideoCaptureMR() {
  if (cap != nullptr)
	  cap->release();
  destroyPort(outputMat);
}
