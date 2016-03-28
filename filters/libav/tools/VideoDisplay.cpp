/*
 * 
 *  Tiny Multimedia Framework (tmf)
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This file is part of tmf.
 *
 *  tmf is free software; you can redistribute it and/or 
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  tmf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with tmf.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "filters/libav/tools/VideoDisplay.h"

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

int VideoDisplay::display(RawFrame * rawFrame) {
  
  if( SDL_PollEvent(&event) && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) 
    return -1; 
  
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
  
  return 0;
}

VideoDisplay::~VideoDisplay() {
}

