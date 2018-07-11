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

#include "video_capture.h"

using namespace df;
using namespace std;

ActorRegister<VideoCapture> VideoCapture::reg("VideoCapture");

VideoCapture::VideoCapture(const string& name) : Actor(name){
  outputMat = createOutputPort<df::Mat>("output");
  cap = nullptr;
}

void VideoCapture::init() {

  if (getProp("file_name") != "")  	
  	file_name = df_path + "/" + getProp("file_name");
  else
        log("error: file_name is not specified.");
  
  cap = new cv::VideoCapture(file_name);

  if(!cap->isOpened()){
    cout << "Error opening video stream or file" << endl;
  }
  *cap >> frame;
}

void VideoCapture::run() {

  df::Mat * out = produce(outputMat);	
  out->set(frame);
  log("capturing frame "+to_string(stepno));

  *cap >> frame;
  if(frame.empty())
    setEos(outputMat);

  release(outputMat);  

  sleep(40);
}
VideoCapture::~VideoCapture() {
  if (cap != nullptr)
	  cap->release();
  destroyPort(outputMat);
}
