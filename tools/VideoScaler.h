/*
 * VideoScaler.h
 *
 *  Created on: Aug 11, 2014
 *      Author: arash
 */

#ifndef VIDEOSCALER_H_
#define VIDEOSCALER_H_

#include "types/Frame.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
}
#endif

class VideoScaler {

private:
	struct SwsContext *sws_ctx;
	int srcWidth;
	int srcHeight;
	int dstWidth;
	int dstHeight;
public:
	VideoScaler(int srcWidth, int srcHeight, AVPixelFormat srcFmt, int dstWidth,
			int dstHeight, AVPixelFormat dstFmt) {

		this->srcWidth = srcWidth;
		this->srcHeight = srcHeight;

		this->dstWidth = dstWidth;
		this->dstHeight = dstHeight;

		sws_ctx = sws_getContext(srcWidth, srcHeight, srcFmt, dstWidth,
				dstHeight, dstFmt,
				SWS_BILINEAR,
				NULL,
				NULL,
				NULL);
	}

	int scale(Frame * inFrame, Frame * outFrame) {

		AVFrame * inAvFrame = inFrame->getFrame();
		AVFrame * outAvFrame = outFrame->getFrame();

		//avcodec_get_frame_defaults(outAvFrame);
		return sws_scale(sws_ctx, (uint8_t const * const *) inAvFrame->data,
				inAvFrame->linesize, 0, srcHeight, outAvFrame->data,
				outAvFrame->linesize);

	}
};

#endif /* VIDEOSCALER_H_ */
