/*
 * 
 *  Tiny Multimedia Framework
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "core/tmf.h"

int main(int argc, char** argv) {
  
  Pipeline* pipe = Factory::createPipeline("Webcam viewer");
  
  Filter* reader = Factory::createFilter("video_reader", "reader");
  Filter* scaler = Factory::createFilter("image_scaler", "scaler");
  Filter* display = Factory::createFilter("video_display", "display");
  
  pipe->connectFilters(reader, scaler);
  pipe->connectFilters(scaler, display);
  
  reader->setProp("input_video", "/dev/video0");
  reader->setProp("video_format", "video4linux2");
  
  scaler->setProp<int>("width", 640);
  scaler->setProp<int>("height", 480);
  
  
  pipe->init();
  
  pipe->run();
  
  Factory::destroyPipeline(pipe);
  
  return 0;
}
