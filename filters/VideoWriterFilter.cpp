/*
 * VideoWriterFilter.cpp
 *
 *  Created on: Aug 18, 2014
 *      Author: arash
 */

#include <filters/VideoWriterFilter.h>

VideoWriterFilter::VideoWriterFilter(string name) :
		Filter(name) {

	inputPortRawFrame = new InputPort<RawFrame>(
			"videoMuxer, input 1, RawFrame");

	inputPorts.push_back(inputPortRawFrame);

	videoWriter = new VideoWriter();

}

FilterStatus VideoWriterFilter::init() {

	MessageError err;
	int width, height;
	//int bitrate;

	string output_video = getProp("output_video");

	err = inMsg->getPropInt("width", width);
	//if (err == MSG_NOT_FOUND)
	//	return FILTER_WAIT_FOR_INPUT;

	err = inMsg->getPropInt("height", height);
	//if (err == MSG_NOT_FOUND)
	//	return FILTER_WAIT_FOR_INPUT;

	//bitrate = 400000;

	videoWriter->init(output_video, width, height);

	return FILTER_SUCCESS;
}
FilterStatus VideoWriterFilter::run() {

  inputPortRawFrame->lock();

  RawFrame * inFrame = inputPortRawFrame->get();

  videoWriter->write(inFrame);
  
  inputPortRawFrame->unlock();

  return FILTER_SUCCESS;
}

VideoWriterFilter::~VideoWriterFilter() {
	delete videoWriter;
	delete inputPortRawFrame;
}

