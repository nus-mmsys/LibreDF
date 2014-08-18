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

#include <filters/VideoEncoderFilter.h>

VideoEncoderFilter::VideoEncoderFilter(string name) :
		Filter(name) {

	inputPortRawFrame = new InputPort<RawFrame>(
			"videoEncoder, input 1, RawFrame", this);
	outputPortEncodedFrame = new OutputPort<EncodedFrame>(
			"videoEncoder, output 1, EncodedFrame", this);

	inputPorts.push_back(inputPortRawFrame);
	outputPorts.push_back(outputPortEncodedFrame);

	videoEncoder = new VideoEncoder();

}

FilterStatus VideoEncoderFilter::init() {

	MessageError err;

	int width, height, bitrate, framerate;

	err = inMsg->getPropInt("width", width);
	if (err == MSG_NOT_FOUND)
		return FILTER_WAIT_FOR_INPUT;

	err = inMsg->getPropInt("height", height);
	if (err == MSG_NOT_FOUND)
		return FILTER_WAIT_FOR_INPUT;

	bitrate = 1000000;
	framerate = 25;

	videoEncoder->init("mpeg4", width, height, bitrate, framerate);

	outMsg->setPropInt("width", width);
	outMsg->setPropInt("height", height);

	return FILTER_SUCCESS;
}

FilterStatus VideoEncoderFilter::process() {

	RawFrame * inFrame = inputPortRawFrame->read();

	EncodedFrame * outFrame =
			outputPortEncodedFrame->getBuffer()->getNextNode();

	videoEncoder->encode(inFrame, outFrame);

	outputPortEncodedFrame->produce(outFrame);
	outputPortEncodedFrame->process();

	return FILTER_SUCCESS;
}

VideoEncoderFilter::~VideoEncoderFilter() {

	delete inputPortRawFrame;
	delete outputPortEncodedFrame;

	delete videoEncoder;
}

