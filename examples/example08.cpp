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

#include "tmffactory/tmf.h"


int main(int argc, char** argv) {
  
  TMF tmf;
  
  if (argc < 3) {
    cerr << "Usage: " << argv[0]
    << " <input video> <width> <height> <output video> " << endl;
    return -1;
  }
  
  Pipeline* pipe = tmf.createPipeline("Reader/Scaler/Encoder/Muxer");
  
  string inputVideo = argv[1];
  string width = argv[2];
  string height = argv[3];
  string outputVideo = argv[4];
  
  Filter* reader = tmf.createFilter(FilterType::VIDEO_READER, "reader");
  Filter* scaler = tmf.createFilter(FilterType::IMAGE_SCALER, "scaler");
  Filter* encoder = tmf.createFilter(FilterType::VIDEO_ENCODER, "encoder");
  Filter* muxer = tmf.createFilter(FilterType::VIDEO_MUXER, "muxer");
  
  pipe->connectFilters(reader, scaler);
  pipe->connectFilters(scaler, encoder);
  pipe->connectFilters(encoder, muxer);
  
  reader->setProp("input_video", inputVideo);
  
  scaler->setProp("width", width);
  scaler->setProp("height", height);
  
  encoder->setProp("bitrate", 1000000);
  encoder->setProp("framerate", 25);
  encoder->setProp("output_video", outputVideo);
  
  pipe->init();
  
  pipe->run();
  
  tmf.destroyPipeline(pipe);
  
  return 0;
}
