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

#include "video_writer.h"

using namespace df;
using namespace std;

ActorRegister<VideoWriter> VideoWriter::reg("VideoWriter");

VideoWriter::VideoWriter(const string& name) : Actor(name) {
  input = createInputPort<df::Mat>("input");
}

void VideoWriter::init() {

  if (getProp("file_name") != "")  	
  	file_name = fsys.outPath() + "/" + getProp("file_name");
  else
	file_name = fsys.outPath() + "/result.avi";
  width = 0;
  height = 0;
}

void VideoWriter::run() {

  auto in = consume(input);	
  frame = in->clone();
  log("writing frame "+to_string(stepno));
  release(input);

  if (width == 0 || height == 0) {
	width = frame.cols;
  	height = frame.rows;
        video = new cv::VideoWriter(file_name,CV_FOURCC('M','J','P','G'),10, cv::Size(width,height));	
  }

  video->write(frame); 

}

VideoWriter::~VideoWriter() {
  destroyPort(input);
  video->release();
}
