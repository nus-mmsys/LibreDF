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

#include "tmf.h"


int main(int argc, char** argv) {
  
  TMF tmf;
  
  if (argc < 8) {
    cerr << "Usage: " << argv[0] << " <input video> <output path1> <width1> <height1> <output path2> <width2> <height2> " << endl;
    return -1;
  }
  
  Pipeline* pipe = tmf.createPipeline("Decoder/2xScaler");
  
  string inputVideo = argv[1];
  string outputPath1 = argv[2];
  string width1 = argv[3];
  string height1 = argv[4];
  
  string outputPath2 = argv[5];
  string width2 = argv[6];
  string height2 = argv[7];
  
  Filter* decoder = tmf.createFilter(FilterType::VIDEO_DECODER, "decoder");
  Filter* scaler1 = tmf.createFilter(FilterType::IMAGE_SCALER, "scaler1");
  Filter* scaler2 = tmf.createFilter(FilterType::IMAGE_SCALER, "scaler2");
  Filter* writer1 = tmf.createFilter(FilterType::IMAGE_WRITER, "writer1");
  Filter* writer2 = tmf.createFilter(FilterType::IMAGE_WRITER, "writer2");
  
  pipe->connectFilters(decoder, scaler1);
  pipe->connectFilters(decoder, scaler2);
  pipe->connectFilters(scaler1,writer1);
  pipe->connectFilters(scaler2,writer2);
  
  decoder->setProp("input_video", inputVideo);
  scaler1->setProp("width", width1);
  scaler1->setProp("height", height1);
  scaler2->setProp("width", width2);
  scaler2->setProp("height", height2);
  
  writer1->setProp("output_path", outputPath1);
  writer2->setProp("output_path", outputPath2);
  
  pipe->init();
  
  pipe->run();
  
  tmf.destroyPipeline(pipe);
  
  return 0;
}
