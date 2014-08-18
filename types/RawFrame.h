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

#ifndef RAW_FRAME_H_
#define RAW_FRAME_H_

#include "types/Data.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#ifdef __cplusplus
}
#endif

class RawFrame : public Data {

private:
	AVFrame * pFrame;
	uint8_t *buffer;

public:
	RawFrame() {
		pFrame = avcodec_alloc_frame();
		buffer=0;
	}

	AVFrame * getFrame() {
		return pFrame;
	}
	void fill(int width, int height, AVPixelFormat format) {

		if(buffer != 0)
			return;
		// Determine required buffer size and allocate buffer
		int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, width, height);
		buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
		// Assign appropriate parts of buffer to image planes in pFrameRGB
		// Note that pFrameRGB is an AVFrame, but AVFrame is a superset
		// of AVPicture
		avpicture_fill((AVPicture *) pFrame, buffer, format, width,
				height);
	}
	~RawFrame() {
		av_free(pFrame);
	}
};


#endif /* RAW_FRAME_H_ */
