/*
 * VideoDisplay.cpp
 *
 *  Created on: Aug 27, 2014
 *      Author: arash
 */

#include <tools/VideoDisplay.h>

VideoDisplay::VideoDisplay() {

	width = 0;
	height = 0;
	screen = 0;
	bmp = 0;
	pSwsCtx = 0;

}

void VideoDisplay::init(int width, int height, AVPixelFormat pixFmt) {

	this->width = width;
	this->height = height;

	screen = SDL_SetVideoMode(width, height, 24, 0);

	bmp = SDL_CreateYUVOverlay(width, height,
				SDL_YV12_OVERLAY, screen);

	pSwsCtx = sws_getContext(width, height,
			pixFmt, width, height,
			PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL);

	rect.x = 0;
	rect.y = 0;
	rect.w = width;
	rect.h = height;

}

void VideoDisplay::display(RawFrame * rawFrame) {

	AVFrame * pFrame = rawFrame->getFrame();

	SDL_LockYUVOverlay(bmp);

	AVPicture pict;
	pict.data[0] = bmp->pixels[0];
	pict.data[1] = bmp->pixels[2];
	pict.data[2] = bmp->pixels[1];

	pict.linesize[0] = bmp->pitches[0];
	pict.linesize[1] = bmp->pitches[2];
	pict.linesize[2] = bmp->pitches[1];

	// Convert the image into YUV format that SDL uses
	sws_scale(pSwsCtx, (const uint8_t * const *) pFrame->data,
			pFrame->linesize, 0, height, pict.data,
			pict.linesize);

	SDL_UnlockYUVOverlay(bmp);

	SDL_DisplayYUVOverlay(bmp, &rect);

}

VideoDisplay::~VideoDisplay() {
	// TODO Auto-generated destructor stub
}

