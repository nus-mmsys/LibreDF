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

#ifndef VIDEODECODER_H_
#define VIDEODECODER_H_

#include "Filter.h"
#include "Port.h"
#include "tools/VideoReader.h"

class VideoDecoderFilter: public Filter {

private:
	VideoReader * videoReader;

	OutputPort<AVFrame*> * outputFrame;
public:
	VideoDecoderFilter(string name) :
			Filter(name) {

		outputFrame = new OutputPort<AVFrame*>(
				"videoDecoder, output 1, AVFrame", this);

		outputPorts.push_back(outputFrame);

		videoReader = 0;
	}

	FilterStatus init() {
		string videoName = getProp("input_video");

		videoReader = new VideoReader(videoName);

		videoReader->dump();

		return FILTER_SUCCESS;
	}

	FilterStatus process() {

		AVFrame * pFrame = outputFrame->getNextNode();
		if (pFrame == 0)
			pFrame = avcodec_alloc_frame();
		int ret = videoReader->readFrame(pFrame);

		if (ret == -1)
			return FILTER_FINISHED;
		//BufferNode<AVFrame> bn(pFrame);

		outputFrame->produce(pFrame);
		outputFrame->process();

		return FILTER_SUCCESS;
	}

	~VideoDecoderFilter() {

		for (int i = 0; i < outputFrame->getBufferSize(); i++) {
			AVFrame * pFrame = outputFrame->getNode(i);
			if (pFrame != 0)
				// Free the YUV frame
				av_free(pFrame);
		}

		delete outputFrame;
		delete videoReader;
	}

};

#endif /* VIDEODECODER_H_ */
