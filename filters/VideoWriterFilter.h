/*
 * VideoWriterFilter.h
 *
 *  Created on: Aug 18, 2014
 *      Author: arash
 */

#ifndef VIDEOWRITERFILTER_H_
#define VIDEOWRITERFILTER_H_

#include "Filter.h"
#include "Port.h"
#include "types/RawFrame.h"
#include "tools/VideoWriter.h"

class VideoWriterFilter : public Filter {
private:
	VideoWriter * videoWriter;

	InputPort<RawFrame> * inputPortRawFrame;
public:
	VideoWriterFilter(string name);
	FilterStatus init();
	FilterStatus process();
	~VideoWriterFilter();
};

#endif /* VIDEOWRITERFILTER_H_ */
