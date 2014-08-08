/*
 * Main.cpp
 *
 *  Created on: Jul 29, 2014
 *      Author: arash
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

	Filter* videoDecoder = tmf.createFilter(VIDEO_DECODER_FILTER, "videoDecoder");
	Filter* imageWriter = tmf.createFilter(IMAGE_WRITER_FILTER, "imageWriter");

	pipe->setProp("input_video", "/home/arash/test_videos/molana.mp4");

	pipe->connectFilters(videoDecoder, imageWriter);

#endif

	pipe->init();
	while (pipe->run() != PIPELINE_FINISHED) {
	}

	return 0;
}
