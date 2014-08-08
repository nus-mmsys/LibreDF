/*
 * Main.cpp
 *
 *  Created on: Jul 29, 2014
 *      Author: arash
 */

#include "Pipeline.h"
#include "FilterFactory.h"

#define APP2

int main(int argc, char** argv) {

	Pipeline* pipe = new Pipeline("DashCast++");

	FilterFactory factory;

#ifdef APP1
	Filter* numberGeneratorFilter = factory.createFilter(NUMBERGENERATOR_FILTER,
			"numberGeneratorFilter");
	Filter* add2Filter = factory.createFilter(ADD2_FILTER, "add2Filter");
	Filter* multiply2Filter = factory.createFilter(MULTIPLY2_FILTER,
			"multiply2Filter");
	Filter* duplicateFilter = factory.createFilter(DUPLICATE_FILTER,
			"duplicateFilter");
	Filter* additionFilter = factory.createFilter(ADDITION_FILTER,
			"additionFilter");

	pipe->connectFilters(numberGeneratorFilter, multiply2Filter);
	pipe->connectFilters(numberGeneratorFilter, add2Filter);
	pipe->connectFilters(numberGeneratorFilter, duplicateFilter);
	pipe->connectFilters(multiply2Filter, additionFilter);
	pipe->connectFilters(add2Filter, additionFilter);

	//pipe->setStarter(numberGeneratorFilter);
#endif

#ifdef APP2

	Filter* videoDecoder = factory.createFilter(VIDEO_DECODER_FILTER, "videoDecoder");
	Filter* imageWriter = factory.createFilter(IMAGE_WRITER_FILTER, "imageWriter");

	pipe->setProp("input_video", "/home/arash/test_videos/molana.mp4");

	pipe->connectFilters(videoDecoder, imageWriter);

	//pipe->setStarter(videoDecoder);


#endif

	pipe->init();
	while (pipe->run() != PIPELINE_FINISHED) {
	}

	return 0;
}
