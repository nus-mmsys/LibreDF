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
  
  if (argc < 5) {
    cerr << "Usage: " << argv[0] << " <input video> <output path> <width> <height>" << endl;
    return -1;
  }
  
  Pipeline* pipe = tmf.createPipeline("Decoder/Scaler");
  
  string inputVideo = argv[1];
  string outputPath = argv[2];
  int width = std::stoi(argv[3]);
  int height = std::stoi(argv[4]);
  
  Filter* reader = tmf.createFilter(FilterType::VIDEO_READER, "reader");
  Filter* scaler = tmf.createFilter(FilterType::IMAGE_SCALER, "scaler");
  Filter* writer = tmf.createFilter(FilterType::IMAGE_WRITER, "writer");
  
  pipe->connectFilters(reader, scaler);
  pipe->connectFilters(scaler,writer);
  
  reader->setProp("input_video", inputVideo);
  
  scaler->setProp<int>("width", width);
  scaler->setProp<int>("height", height);
  
  writer->setProp("output_path", outputPath);
  
  pipe->init();
  
  pipe->run();
  
  tmf.destroyPipeline(pipe);
  
  return 0;
}
