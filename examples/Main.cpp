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

#define APP6

int main(int argc, char** argv) {
  
  TMF tmf;
  
  #ifdef APP1
  
  Pipeline* pipe = tmf.createPipeline("Test App");
  
  Filter* numberGeneratorFilter = tmf.createFilter(NUMBERGENERATOR_FILTER,
						   "numberGeneratorFilter");
  Filter* add2Filter = tmf.createFilter(ADD2_FILTER, "add2Filter");
  Filter* multiply2Filter = tmf.createFilter(MULTIPLY2_FILTER,
					     "multiply2Filter");
  Filter* duplicateFilter = tmf.createFilter(DUPLICATE_FILTER,
					     "duplicateFilter");
  Filter* additionFilter = tmf.createFilter(ADDITION_FILTER,
					    "additionFilter");
  
  pipe->connectFilters(numberGeneratorFilter, multiply2Filter);
  pipe->connectFilters(numberGeneratorFilter, add2Filter);
  pipe->connectFilters(numberGeneratorFilter, duplicateFilter);
  pipe->connectFilters(multiply2Filter, additionFilter);
  pipe->connectFilters(add2Filter, additionFilter);
  
  #endif
  
  #ifdef APP2
  
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
  
  #endif
  
  #ifdef APP3
  
  if (argc < 5) {
    cerr << "Usage: " << argv[0] << " <input video> <output path> <width> <height>" << endl;
    return -1;
  }
  
  Pipeline* pipe = tmf.createPipeline("Decoder/Scaler");
  
  string inputVideo = argv[1];
  string outputPath = argv[2];
  int width = std::stoi(argv[3]);
  int height = std::stoi(argv[4]);
  
  Filter* videoDecoder = tmf.createFilter(VIDEO_DECODER_FILTER,
					  "videoDecoder");
  Filter* imageScaler1 = tmf.createFilter(IMAGE_SCALER_FILTER,
					  "imageScaler1");
  Filter* imageWriter = tmf.createFilter(IMAGE_WRITER_FILTER, "imageWriter");
  
  pipe->connectFilters(videoDecoder, imageScaler1);
  pipe->connectFilters(imageScaler1,imageWriter);
  
  videoDecoder->setProp("input_video", inputVideo);
  
  imageScaler1->setProp("width", to_string(width));
  imageScaler1->setProp("height", to_string(height));
  
  imageWriter->setProp("output_path", outputPath);
  
  #endif
  
  #ifdef APP4
  
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
  
  Filter* videoDecoder = tmf.createFilter(VIDEO_DECODER_FILTER,
					  "videoDecoder");
  Filter* imageScaler1 = tmf.createFilter(IMAGE_SCALER_FILTER,
					  "imageScaler1");
  Filter* imageScaler2 = tmf.createFilter(IMAGE_SCALER_FILTER,
					  "imageScaler2");
  Filter* imageWriter1 = tmf.createFilter(IMAGE_WRITER_FILTER, "imageWriter1");
  Filter* imageWriter2 = tmf.createFilter(IMAGE_WRITER_FILTER, "imageWriter2");
  
  pipe->connectFilters(videoDecoder, imageScaler1);
  pipe->connectFilters(videoDecoder, imageScaler2);
  pipe->connectFilters(imageScaler1,imageWriter1);
  pipe->connectFilters(imageScaler2,imageWriter2);
  
  videoDecoder->setProp("input_video", inputVideo);
  imageScaler1->setProp("width", width1);
  imageScaler1->setProp("height", height1);
  imageScaler2->setProp("width", width2);
  imageScaler2->setProp("height", height2);
  
  imageWriter1->setProp("output_path", outputPath1);
  imageWriter2->setProp("output_path", outputPath2);
  
  #endif
  
  #ifdef APP5
  
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
  
  #endif
  
  #ifdef APP6
  
  if (argc < 2) {
    cerr << "Usage: " << argv[0]
    << " <input video> " << endl;
    return -1;
  }
  
  Pipeline* pipe = tmf.createPipeline("Media Player");
  
  string inputVideo = argv[1];
  
  Filter* videoDecoder = tmf.createFilter(VIDEO_DECODER_FILTER,
					  "videoDecoder");
  
  Filter* videoDisplay = tmf.createFilter(VIDEO_DISPLAY_FILTER, "videoDisplay");
  
  pipe->connectFilters(videoDecoder, videoDisplay);
  
  videoDecoder->setProp("input_video", inputVideo);
  
  #endif
  
  pipe->init();
  
  pipe->run();
  
  pipe->wait();
  
  tmf.destroyPipeline(pipe);
  
  return 0;
}
