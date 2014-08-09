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

#ifndef IMAGEWRITER_H_
#define IMAGEWRITER_H_

#include "Filter.h"
#include "tools/VideoFrameWriter.h"

class ImageWriterFilter: public Filter {

private:

	VideoFrameWriter * videoFrameWriter;


	InputPort<AVFrame*> * inputFrame;
public:
	ImageWriterFilter(string name) :
			Filter(name) {

		inputFrame = new InputPort<AVFrame*>("imageWriter, input 1, AVFrame",
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
		videoFrameWriter->setPath(getProp("output_path"));

		return FILTER_SUCCESS;
	}

	FilterStatus process() {

		AVFrame * pFrame = inputFrame->read();

		videoFrameWriter->writeImage(pFrame);

		return FILTER_SUCCESS;
	}

	~ImageWriterFilter() {
	}

};

#endif /* IMAGEWRITER_H_ */
