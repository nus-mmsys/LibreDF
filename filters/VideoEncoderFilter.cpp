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
			"videoEncoder, input 1, RawFrame");
	outputPortEncodedFrame = new OutputPort<EncodedFrame>(
			"videoEncoder, output 1, EncodedFrame");

	inputPorts.push_back(inputPortRawFrame);
	outputPorts.push_back(outputPortEncodedFrame);

	videoEncoder = new VideoEncoder();

}

void VideoEncoderFilter::init() {

	MessageError err;

	int width, height, bitrate, framerate;

	string output_video = getProp("output_video");

	bitrate = std::stoi(getProp("bitrate"));

	framerate = std::stoi(getProp("framerate"));

	err = inMsg->getPropInt("width", width);
	//if (err == MSG_NOT_FOUND)
	//	return FILTER_WAIT_FOR_INPUT;

	err = inMsg->getPropInt("height", height);
	//if (err == MSG_NOT_FOUND)
	//	return FILTER_WAIT_FOR_INPUT;

	videoEncoder->init(output_video, width, height, bitrate, framerate);

	outMsg->setPropInt("width", width);
	outMsg->setPropInt("height", height);
	outMsg->setPropInt("bitrate", bitrate);
	outMsg->setPropInt("framerate", framerate);
	outMsg->setProp("output_video", output_video);

}

void VideoEncoderFilter::run() {
  
  
  inputPortRawFrame->lock();
  
  RawFrame * inFrame = inputPortRawFrame->get();

  outputPortEncodedFrame->lock();
  
  EncodedFrame * outFrame = outputPortEncodedFrame->get();
  videoEncoder->encode(inFrame, outFrame);

  outputPortEncodedFrame->unlock();

  inputPortRawFrame->unlock();

}

VideoEncoderFilter::~VideoEncoderFilter() {

	delete inputPortRawFrame;
	delete outputPortEncodedFrame;

	delete videoEncoder;
}

