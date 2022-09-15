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

#include "video_capture_split.h"

using namespace df;
using namespace std;

ActorRegister<VideoCaptureSplit> VideoCaptureSplit::reg("VideoCaptureSplit");

VideoCaptureSplit::VideoCaptureSplit(const string& name) : Actor(name){
  output = createOutputPortVector<df::Mat>("output");
  cap = nullptr;
}

void VideoCaptureSplit::init() {

  if (getProp("file_name") != "")  	
  	file_name = fsys.inPath() + "/" + getProp("file_name");
  else
        log("error: file_name is not specified.");

  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");

  cap = new cv::VideoCapture(file_name);

  if(!cap->isOpened()){
    cout << "Error opening video stream or file" << endl;
  }
  *cap >> frame;

}

void VideoCaptureSplit::reinit() {
  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");
}

void VideoCaptureSplit::run() {

  auto out = produce(output);
 
  for (int j=0; j < arity ; j++) {
	  out[j]->set(frame);
          *cap >> frame;
          if(frame.empty()) {
		  while (j<arity) {
			  out[j]->set(*out[0]->get());
			  j++;
		  }
		  setEos(output);
		  break;
	  }
  }
  
  log("capturing frame "+to_string(stepno));

  release(output);
 
}
VideoCaptureSplit::~VideoCaptureSplit() {
  if (cap != nullptr)
	  cap->release();
  destroyPort(output);
}
