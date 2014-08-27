/*
 * VideoDisplay.h
 *
 *  Created on: Aug 27, 2014
 *      Author: arash
 */

#ifndef VIDEODISPLAY_H_
#define VIDEODISPLAY_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
}
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

#include "types/RawFrame.h"

class VideoDisplay {
private:
	int width, height;
	SDL_Overlay *bmp;
	SDL_Surface *screen;
	SDL_Rect rect;
	struct SwsContext * pSwsCtx;
public:
	VideoDisplay();
	void init(int width, int height, AVPixelFormat pixFmt);
	void display(RawFrame * rawFrame);
	virtual ~VideoDisplay();
};

#endif /* VIDEODISPLAY_H_ */
