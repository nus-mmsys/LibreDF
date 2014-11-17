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
  
  
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " <input video> <output path>" << endl;
    return -1;
  }
  
  Pipeline* pipe = tmf.createPipeline("Decoder");
  
  string inputVideo = argv[1];
  string outputPath = argv[2];
  
  Filter* videoDecoder = tmf.createFilter(VIDEO_DECODER_FILTER,
					  "videoDecoder");
  Filter* imageWriter = tmf.createFilter(IMAGE_WRITER_FILTER, "imageWriter");
  
  pipe->connectFilters(videoDecoder, imageWriter);
  
  videoDecoder->setProp("input_video", inputVideo);
  imageWriter->setProp("output_path", outputPath);
  
  pipe->init();
  
  pipe->run();
  
  pipe->wait();
  
  tmf.destroyPipeline(pipe);
  
  return 0;
}
