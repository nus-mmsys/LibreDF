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

	pipe->init();

	pipe->run();

	tmf.destroyPipeline(pipe);

	return 0;
}
