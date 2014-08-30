/*
 * VideoDisplayFilter.h
 *
 *  Created on: Aug 27, 2014
 *      Author: arash
 */

#ifndef VIDEODISPLAYFILTER_H_
#define VIDEODISPLAYFILTER_H_

#include "include/Filter.h"
#include "include/Port.h"
#include "types/RawFrame.h"
#include "tools/VideoDisplay.h"

class VideoDisplayFilter : public Filter {

private:
	VideoDisplay * videoDisplay;

	InputPort<RawFrame> * inputPortRawFrame;
public:
	VideoDisplayFilter(string name);
	FilterStatus init();
	FilterStatus process();
	~VideoDisplayFilter();
};

#endif /* VIDEODISPLAYFILTER_H_ */