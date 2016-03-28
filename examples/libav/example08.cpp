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
  
  if (argc < 3) {
    cerr << "Usage: " << argv[0]
    << " <input video> <width> <height> <output video> " << endl;
    return -1;
  }
  
  Pipeline* pipe = Factory::createPipeline("Reader/Scaler/Encoder/Muxer");
  
  string inputVideo = argv[1];
  string width = argv[2];
  string height = argv[3];
  string outputVideo = argv[4];
  
  Filter* reader = Factory::createFilter("video_reader", "reader");
  Filter* scaler = Factory::createFilter("image_scaler", "scaler");
  Filter* encoder = Factory::createFilter("video_encoder", "encoder");
  Filter* muxer = Factory::createFilter("video_muxer", "muxer");
  
  pipe->addFilters(reader, scaler, encoder, muxer, nullptr);
  
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
  
  Factory::destroyPipeline(pipe);
  
  return 0;
}
