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

#ifndef IMAGEWRITER_H_
#define IMAGEWRITER_H_

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

#include "filters/libav/types/RawFrame.h"

using namespace std;

class ImageWriter {
  
private:
  string path;
  struct SwsContext *sws_ctx;
  AVFrame * pFrameRGB;
  uint8_t *buffer;
  int width;
  int height;
  AVPixelFormat format;
  //int frameNumber;
  
public:
  
  ImageWriter(int width, int height, AVPixelFormat format);
  
  void writeImage(RawFrame * frame);
  
  void setPath(string path);
  
  ~ImageWriter();
  
};

#endif /* IMAGEWRITER_H_ */
