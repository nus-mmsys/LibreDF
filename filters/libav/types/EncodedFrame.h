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

#ifndef ENCODEDFRAME_H_
#define ENCODEDFRAME_H_

#include "filters/libav/types/Frame.h"

#ifdef __cplusplus
extern "C" {
  #endif
  #include <libavutil/avutil.h>
  #include <libavcodec/avcodec.h>
  #include <libavformat/avformat.h>
  #ifdef __cplusplus
}
#endif

class EncodedFrame: public Frame {
  
public:
  AVPacket * data;
  int vbuf_size;
  uint8_t * vbuf;
public:
  EncodedFrame() {
    data = (AVPacket *) av_malloc(sizeof(AVPacket));
    vbuf_size = 9 * 1920 * 1080 + 10000;
    vbuf = (uint8_t *) av_malloc(vbuf_size);
  
    data->size = vbuf_size;
    data->data = vbuf;
  }
  AVPacket * getPacket() {
    return data;
  }
  
  ~EncodedFrame() {
    av_free(vbuf);
    av_free_packet(data);
  }
  
};

#endif /* ENCODEDFRAME_H_ */
