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

#define APP2

int main(int argc, char** argv) {

	TMF tmf;

	Pipeline* pipe = tmf.createPipeline("DashCast++");

#ifdef APP1
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
		cerr << "Usage: " << argv[0] << " <input video> <output path>";
		return -1;
	}

	string inputVideo = argv[1];
	string outputPath = argv[2];

	Filter* videoDecoder = tmf.createFilter(VIDEO_DECODER_FILTER,
			"videoDecoder");
	Filter* imageWriter = tmf.createFilter(IMAGE_WRITER_FILTER, "imageWriter");

	pipe->setProp("input_video", inputVideo);
	pipe->setProp("output_path", outputPath);

	pipe->connectFilters(videoDecoder, imageWriter);

#endif

	pipe->init();

	pipe->run();

	tmf.destroyPipeline(pipe);

	return 0;
}
