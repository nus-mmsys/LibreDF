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

	pipe->init();

	pipe->run();

	tmf.destroyPipeline(pipe);

	return 0;
}
