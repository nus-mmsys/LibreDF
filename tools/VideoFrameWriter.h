/*
 * VideoFrame.h
 *
 *  Created on: Aug 8, 2014
 *      Author: arash
 */

#ifndef VIDEOFRAME_H_
#define VIDEOFRAME_H_

#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
}
#endif

using namespace std;

class VideoFrameWriter {

private:
	struct SwsContext *sws_ctx;
	AVFrame * pFrameRGB;
	int width;
	int height;
	AVPixelFormat format;
	int frameNumber;

public:

	VideoFrameWriter(int width, int height, AVPixelFormat format) {

		int numBytes;
		uint8_t *buffer = 0;

		this->width = width;
		this->height = height;
		this->format = format;
		frameNumber = 0;

		// Allocate an AVFrame structure
		pFrameRGB = avcodec_alloc_frame();

		if (pFrameRGB == NULL) {
			std::cerr << "Cannot allocate frame." << endl;

		}

		// Determine required buffer size and allocate buffer
		numBytes = avpicture_get_size(PIX_FMT_RGB24, width, height);
		buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));

		sws_ctx = sws_getContext(width, height, format, width, height,
				AV_PIX_FMT_RGB24,
				SWS_BILINEAR,
				NULL,
				NULL,
				NULL);

		// Assign appropriate parts of buffer to image planes in pFrameRGB
		// Note that pFrameRGB is an AVFrame, but AVFrame is a superset
		// of AVPicture
		avpicture_fill((AVPicture *) pFrameRGB, buffer, PIX_FMT_RGB24, width,
				height);

	}

	void writeImage(AVFrame * pFrame) {

		sws_scale(sws_ctx, (uint8_t const * const *) pFrame->data,
				pFrame->linesize, 0, height, pFrameRGB->data,
				pFrameRGB->linesize);

		FILE *pFile;
		char szFilename[32];
		int y;

		// Open file
		sprintf(szFilename, "frame%d.ppm", frameNumber++);
		pFile = fopen(szFilename, "wb");
		if (pFile == NULL)
			return;

		// Write header
		fprintf(pFile, "P6\n%d %d\n255\n", width, height);

		// Write pixel data
		for (y = 0; y < height; y++)
			fwrite(pFrame->data[0] + y * pFrame->linesize[0], 1, width * 3,
					pFile);

		// Close file
		fclose(pFile);

	}

};

#endif /* VIDEOFRAME_H_ */
