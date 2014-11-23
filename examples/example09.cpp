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
  
  if (argc < 2) {
    cerr << "Usage: " << argv[0]
    << " <input video> " << endl;
    return -1;
  }
  
  Pipeline* pipe = Factory::createPipeline("Media Player");
  
  string inputVideo = argv[1];
  
  Filter* reader = Factory::createFilter("video_reader", "reader");
  
  Filter* display = Factory::createFilter("video_display", "display");
  
  pipe->connectFilters(reader, display);
  
  reader->setProp("input_video", inputVideo);
  
  pipe->init();
  
  pipe->run();
  
  Factory::destroyPipeline(pipe);
  
  return 0;
}
