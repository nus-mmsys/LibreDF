/*
 * VideoDisplayFilter.h
 *
 *  Created on: Aug 27, 2014
 *      Author: arash
 */

#ifndef VIDEODISPLAYFILTER_H_
#define VIDEODISPLAYFILTER_H_

#include "core/Filter.h"
#include "core/Port.h"
#include "types/RawFrame.h"
#include "tools/VideoDisplay.h"

class VideoDisplayFilter : public Filter {

private:
	VideoDisplay * videoDisplay;

	InputPort<RawFrame> * inputPortRawFrame;
public:
	VideoDisplayFilter(string name);
	FilterStatus init();
	FilterStatus run();
	~VideoDisplayFilter();
};

#endif /* VIDEODISPLAYFILTER_H_ */
