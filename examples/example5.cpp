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
    cerr << "Usage: " << argv[0]
    << " <input video> <width> <height> <output video> " << endl;
    return -1;
  }
  
  Pipeline* pipe = tmf.createPipeline("Reader/Scaler/Encoder/Muxer");
  
  string inputVideo = argv[1];
  string width = argv[2];
  string height = argv[3];
  string outputVideo = argv[4];
  
  Filter* videoDecoder = tmf.createFilter(VIDEO_DECODER_FILTER,
					  "videoDecoder");
  Filter* imageScaler = tmf.createFilter(IMAGE_SCALER_FILTER, "imageScaler");
  Filter* videoEncoder = tmf.createFilter(VIDEO_ENCODER_FILTER,
					  "videoEncoder");
  Filter* videoMuxer = tmf.createFilter(VIDEO_MUXER_FILTER, "videoMuxer");
  
  pipe->connectFilters(videoDecoder, imageScaler);
  pipe->connectFilters(imageScaler, videoEncoder);
  pipe->connectFilters(videoEncoder, videoMuxer);
  
  videoDecoder->setProp("input_video", inputVideo);
  
  imageScaler->setProp("width", width);
  imageScaler->setProp("height", height);
  
  videoEncoder->setProp("bitrate", "1000000");
  videoEncoder->setProp("framerate", "25");
  videoEncoder->setProp("output_video", outputVideo);
  
  pipe->init();
  
  pipe->run();
  pipe->wait();
  tmf.destroyPipeline(pipe);
  
  return 0;
}
