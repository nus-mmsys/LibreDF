/*
 * VideoDisplayFilter.cpp
 *
 *  Created on: Aug 27, 2014
 *      Author: arash
 */

#include <filters/VideoDisplayFilter.h>

VideoDisplayFilter::VideoDisplayFilter(string name) :
		Filter(name) {

	inputPortRawFrame = new InputPort<RawFrame>(
			"videoEncoder, input 1, RawFrame");

	inputPorts.push_back(inputPortRawFrame);

	videoDisplay = new VideoDisplay();

}

FilterStatus VideoDisplayFilter::init() {

	MessageError err;
	int width, height, pixFmtInt;

	err = inMsg->getPropInt("width", width);
	if (err == MSG_NOT_FOUND)
		return FILTER_WAIT_FOR_INPUT;

	err = inMsg->getPropInt("height", height);
	if (err == MSG_NOT_FOUND)
		return FILTER_WAIT_FOR_INPUT;

	err = inMsg->getPropInt("format", pixFmtInt);
	if (err == MSG_NOT_FOUND)
		return FILTER_WAIT_FOR_INPUT;

	AVPixelFormat pixFmt = static_cast<AVPixelFormat>(pixFmtInt);

	videoDisplay->init(width, height, pixFmt);

	return FILTER_SUCCESS;
}
FilterStatus VideoDisplayFilter::process() {

	RawFrame * inFrame = (RawFrame *) inputPortRawFrame->get();

	videoDisplay->display(inFrame);

	return FILTER_SUCCESS;
}

VideoDisplayFilter::~VideoDisplayFilter() {
	// TODO Auto-generated destructor stub
}

