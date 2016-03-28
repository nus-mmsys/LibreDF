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
  
  if (argc < 5) {
    cerr << "Usage: " << argv[0] << " <input video> <output path> <width> <height>" << endl;
    return -1;
  }
  
  Pipeline* pipe = Factory::createPipeline("Decoder/Scaler");
  
  string inputVideo = argv[1];
  string outputPath = argv[2];
  int width = std::stoi(argv[3]);
  int height = std::stoi(argv[4]);
  
  Filter* reader = Factory::createFilter("video_reader", "reader");
  Filter* scaler = Factory::createFilter("image_scaler", "scaler");
  Filter* writer = Factory::createFilter("image_writer", "writer");
  
  pipe->addFilters(reader, scaler, writer, nullptr);
  
  pipe->connectFilters(reader, scaler);
  pipe->connectFilters(scaler,writer);
  
  reader->setProp("input_video", inputVideo);
  
  scaler->setProp<int>("width", width);
  scaler->setProp<int>("height", height);
  
  writer->setProp("output_path", outputPath);
  
  pipe->init();
  
  pipe->run();
  
  Factory::destroyPipeline(pipe);
  
  return 0;
}
