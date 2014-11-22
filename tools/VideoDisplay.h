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
