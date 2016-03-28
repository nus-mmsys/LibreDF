/*
 * 
 *  Tiny Multimedia Framework (tmf)
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This file is part of tmf.
 *
 *  tmf is free software; you can redistribute it and/or 
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  tmf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with tmf.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "core/tmf.h"

using namespace tmf;
using namespace std;

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
  
  pipe->addFilters(reader, display, nullptr);
  
  pipe->connectFilters(reader, display);
  
  reader->setProp("input_video", inputVideo);
  
  pipe->init();
  
  pipe->run();
  
  Factory::destroyPipeline(pipe);
  
  return 0;
}
