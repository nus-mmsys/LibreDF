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

#include "video_capture_slice.h"

using namespace df;
using namespace std;

ActorRegister<VideoCaptureSlice> VideoCaptureSlice::reg("VideoCaptureSlice");

VideoCaptureSlice::VideoCaptureSlice(const string& name) : Actor(name){
  output = createOutputPortVector<df::Mat>("output");
  cap = nullptr;
}

void VideoCaptureSlice::init() {

  if (getProp("file_name") != "")  	
  	file_name = fsys.inPath() + "/" + getProp("file_name");
  else
        log("error: file_name is not specified.");

  if (propEmpty("level"))
	  level = 1;
  else
	  level = getPropInt("level");

  tilew = 0;
  tileh = 0;

  output->setArity(level * level);

  cap = new cv::VideoCapture(file_name);

  if(!cap->isOpened()){
    cout << "Error opening video stream or file" << endl;
  }
  *cap >> frame;

  tilew = frame.cols / level;
  tileh = frame.rows / level;
}

void VideoCaptureSlice::run() {

  auto out = produce(output);
 
  for (int j=0; j < level ; j++) {
	  for (int i=0; i < level ; i++) {
	  	cv::Rect tile(i * tilew,
				j * tileh,
				tilew, tileh);
  	  	out[j*level+i]->set(frame(tile));
	  }
  }
  
  log("capturing frame "+to_string(stepno));

  *cap >> frame;
  if(frame.empty())
    setEos(output);

  release(output);
 
}
VideoCaptureSlice::~VideoCaptureSlice() {
  if (cap != nullptr)
	  cap->release();
  destroyPort(output);
}
