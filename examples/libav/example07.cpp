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
  
  if (argc < 8) {
    cerr << "Usage: " << argv[0] << " <input video> <output path1> <width1> <height1> <output path2> <width2> <height2> " << endl;
    return -1;
  }
  
  Pipeline* pipe = Factory::createPipeline("Decoder/2xScaler");
  
  string inputVideo = argv[1];
  string outputPath1 = argv[2];
  string width1 = argv[3];
  string height1 = argv[4];
  
  string outputPath2 = argv[5];
  string width2 = argv[6];
  string height2 = argv[7];
  
  Filter* reader = Factory::createFilter("video_reader", "reader");
  Filter* scaler1 = Factory::createFilter("image_scaler", "scaler1");
  Filter* scaler2 = Factory::createFilter("image_scaler", "scaler2");
  Filter* writer1 = Factory::createFilter("image_writer", "writer1");
  Filter* writer2 = Factory::createFilter("image_writer", "writer2");
  
  pipe->addFilters(reader, scaler1, scaler2, writer1, writer2, nullptr);
  
  pipe->connectFilters(reader, scaler1);
  pipe->connectFilters(reader, scaler2);
  pipe->connectFilters(scaler1,writer1);
  pipe->connectFilters(scaler2,writer2);
  
  reader->setProp("input_video", inputVideo);
  scaler1->setProp("width", width1);
  scaler1->setProp("height", height1);
  scaler2->setProp("width", width2);
  scaler2->setProp("height", height2);
  
  writer1->setProp("output_path", outputPath1);
  writer2->setProp("output_path", outputPath2);
  
  pipe->init();
  
  pipe->run();
  
  Factory::destroyPipeline(pipe);
  
  return 0;
}
