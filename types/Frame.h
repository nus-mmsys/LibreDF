/*
 * Frame.h
 *
 *  Created on: Aug 9, 2014
 *      Author: arash
 */

#ifndef FRAME_H_
#define FRAME_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#ifdef __cplusplus
}
#endif

class Frame {

private:
	AVFrame * pFrame;
	uint8_t *buffer;

public:
	Frame() {
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
	~Frame() {
		av_free(pFrame);
	}
};


#endif /* FRAME_H_ */
