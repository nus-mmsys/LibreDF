/*
 * ImageWriter.h
 *
 *  Created on: Aug 7, 2014
 *      Author: arash
 */

#ifndef IMAGEWRITER_H_
#define IMAGEWRITER_H_

#include "Filter.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
}
#endif

class ImageWriter: public Filter {

private:
	AVFrame *pFrameRGB;

	InputPort<AVFrame> * inputFrame;
public:
	ImageWriter(string name) :
			Filter(name) {

		inputFrame = new InputPort<AVFrame>("imageWriter, input 1, AVPacket",
				this);

		inputPorts.push_back(inputFrame);

		pFrameRGB = 0;
	}

	FilterStatus init() {

		// Allocate an AVFrame structure
		pFrameRGB = avcodec_alloc_frame();

		if (pFrameRGB == NULL) {
			std::cerr << "Cannot allocate frame." << endl;
			return FILTER_ERROR;
		}

		/*
		// Determine required buffer size and allocate buffer
		numBytes = avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,
				pCodecCtx->height);
		buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));

		sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
				pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
				PIX_FMT_RGB24,
				SWS_BILINEAR,
				NULL,
				NULL,
				NULL);

		// Assign appropriate parts of buffer to image planes in pFrameRGB
		// Note that pFrameRGB is an AVFrame, but AVFrame is a superset
		// of AVPicture
		avpicture_fill((AVPicture *) pFrameRGB, buffer, PIX_FMT_RGB24,
				pCodecCtx->width, pCodecCtx->height);


		); */

		return FILTER_SUCCESS;
	}

	FilterStatus process() {

		/*
		BufferNode<AVFrame> * bn = inputFrame->read();


		AVFrame * vframe = bn->getData();

		sws_scale(sws_ctx, (uint8_t const * const *) pFrame->data,
				pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,
				pFrameRGB->linesize);

		// Save the frame to disk
		if(++i<=5)
			SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height,i);

	*/
			return FILTER_SUCCESS;
	}

	~ImageWriter() {
	}

};

#endif /* IMAGEWRITER_H_ */
