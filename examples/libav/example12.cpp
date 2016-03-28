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

using namespace std;
using namespace tmf;

int main(int argc, char** argv) {
  
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " <output video 1> <output video 2> " << endl;
    return -1;
  }
  
  string outputVideo1 = argv[1];
  string outputVideo2 = argv[2];
 
  Pipeline* pipe = Factory::createPipeline("DASH webcam stream - two output");
  
  Filter* reader = Factory::createFilter("video_reader", "reader");
  Filter* scaler1 = Factory::createFilter("image_scaler", "scaler1");
  Filter* scaler2 = Factory::createFilter("image_scaler", "scaler2");
  Filter* encoder1 = Factory::createFilter("video_encoder", "encoder1");
  Filter* encoder2 = Factory::createFilter("video_encoder", "encoder2");
  Filter* muxer1 = Factory::createFilter("video_muxer", "muxer1");
  Filter* muxer2 = Factory::createFilter("video_muxer", "muxer2");
  
  pipe->addFilters(reader, scaler1, scaler2, encoder1, encoder2, muxer1, muxer2, nullptr);
  
  pipe->connectFilters(reader, scaler1);
  pipe->connectFilters(scaler1, encoder1);
  pipe->connectFilters(encoder1, muxer1);
  pipe->connectFilters(reader, scaler2);
  pipe->connectFilters(scaler2, encoder2);
  pipe->connectFilters(encoder2, muxer2);
  
  reader->setProp("input_video", "/dev/video0");
  reader->setProp("video_format", "video4linux2");
  reader->setProp("input_format", "mjpeg");
  reader->setProp("framerate", "20");
  
  scaler1->setProp<int>("width", 640);
  scaler1->setProp<int>("height", 480);
  
  scaler2->setProp<int>("width", 320);
  scaler2->setProp<int>("height", 240);
  
  encoder1->setProp("bitrate", 400000);
  encoder1->setProp("framerate", 20);
  encoder1->setProp("output_video", outputVideo1);
 
  encoder2->setProp("bitrate", 400000);
  encoder2->setProp("framerate", 20);
  encoder2->setProp("output_video", outputVideo2);
  
  pipe->init();
  
  pipe->run();
  
  Factory::destroyPipeline(pipe);
  
  return 0;
}
