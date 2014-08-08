/*
 * ImageWriter.h
 *
 *  Created on: Aug 7, 2014
 *      Author: arash
 */

#ifndef IMAGEWRITER_H_
#define IMAGEWRITER_H_

#include "Filter.h"
#include "tools/VideoFrameWriter.h"

class ImageWriter: public Filter {

private:

	VideoFrameWriter * videoFrameWriter;

	InputPort<AVFrame> * inputFrame;
public:
	ImageWriter(string name) :
			Filter(name) {

		inputFrame = new InputPort<AVFrame>("imageWriter, input 1, AVFrame",
				this);

		inputPorts.push_back(inputFrame);

		videoFrameWriter = 0;
	}

	FilterStatus init() {

		string videoName = getProp("input_video");

		VideoReader media = VideoReader(videoName);
		int width = media.getCodecContext()->width;
		int height = media.getCodecContext()->height;
		AVPixelFormat format = media.getCodecContext()->pix_fmt;

		videoFrameWriter = new VideoFrameWriter(width, height, format);

		return FILTER_SUCCESS;
	}

	FilterStatus process() {

		BufferNode<AVFrame> * bn = inputFrame->read();

		AVFrame * pFrame = bn->getData();

		videoFrameWriter->writeImage(pFrame);

		return FILTER_SUCCESS;
	}

	~ImageWriter() {
	}

};

#endif /* IMAGEWRITER_H_ */
