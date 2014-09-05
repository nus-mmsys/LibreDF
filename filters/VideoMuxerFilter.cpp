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

#include <filters/VideoMuxerFilter.h>

VideoMuxerFilter::VideoMuxerFilter(string name) :
		Filter(name) {

	inputPortEncodedFrame = new InputPort<EncodedFrame>(
			"videoMuxer, input 1, EncodedFrame");

	inputPorts.push_back(inputPortEncodedFrame);

	videoMuxer = new VideoMuxer();

}

FilterStatus VideoMuxerFilter::init() {

	MessageError err;
	string output_video;
	int width, height, bitrate, framerate;

	err = inMsg->getPropString("output_video", output_video);
	if (err == MSG_NOT_FOUND)
		return FILTER_WAIT_FOR_INPUT;

	err = inMsg->getPropInt("width", width);
	if (err == MSG_NOT_FOUND)
		return FILTER_WAIT_FOR_INPUT;

	err = inMsg->getPropInt("height", height);
	if (err == MSG_NOT_FOUND)
		return FILTER_WAIT_FOR_INPUT;

	err = inMsg->getPropInt("bitrate", bitrate);
	if (err == MSG_NOT_FOUND)
		return FILTER_WAIT_FOR_INPUT;

	err = inMsg->getPropInt("framerate", framerate);
	if (err == MSG_NOT_FOUND)
		return FILTER_WAIT_FOR_INPUT;

	videoMuxer->init(output_video, width, height, bitrate, framerate);

	return FILTER_SUCCESS;
}

FilterStatus VideoMuxerFilter::process() {

	EncodedFrame * inFrame = inputPortEncodedFrame->read();

	videoMuxer->mux(inFrame);

	return FILTER_SUCCESS;
}

VideoMuxerFilter::~VideoMuxerFilter() {
	delete inputPortEncodedFrame;

	delete videoMuxer;
}

