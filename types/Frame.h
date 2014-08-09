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

public:
	Frame() {
		pFrame = avcodec_alloc_frame();
	}

	AVFrame * getFrame() {
		return pFrame;
	}
	~Frame() {
		av_free(pFrame);
	}
};


#endif /* FRAME_H_ */
