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

	OutputPort<AVFrame> * outputFrame;
public:
	VideoDecoderFilter(string name) :
			Filter(name) {

		outputFrame = new OutputPort<AVFrame>("videoDecoder, output 1, AVFrame",
				this);

		outputPorts.push_back(outputFrame);

		videoReader = 0;
	}

	FilterStatus init() {
		string videoName = getProp("input_video");

		videoReader = new VideoReader(videoName);

		//AVCodecContext * pCodecCtx = videoReader->getCodecContext();

		//cout << "w: " << pCodecCtx->width << ", h: " << pCodecCtx->height
		//		<< ", p: " << pCodecCtx->pix_fmt << endl;
		return FILTER_SUCCESS;
	}

	FilterStatus process() {
		AVFrame * pFrame = videoReader->readFrame();

		BufferNode<AVFrame> bn(pFrame);

		outputFrame->produce(&bn);
		outputFrame->process();

		return FILTER_SUCCESS;
	}

	~VideoDecoderFilter() {
	}

};

#endif /* VIDEODECODER_H_ */
