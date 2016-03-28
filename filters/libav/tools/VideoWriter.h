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

#ifndef VIDEOWRITER_H_
#define VIDEOWRITER_H_

#include <string>
#include "filters/libav/types/RawFrame.h"
#include "filters/libav/types/EncodedFrame.h"

using namespace std;

#ifdef __cplusplus
extern "C" {
  #endif
  #include <libavcodec/avcodec.h>
  #include <libavformat/avformat.h>
  #include <libavutil/opt.h>
  #include <libavutil/mathematics.h>
  #ifdef __cplusplus
}
#endif

class VideoWriter {
private:
  AVCodec *codec;
  AVFormatContext *oc;
  AVStream *video_st;
  uint8_t *video_outbuf;
  int video_outbuf_size;
  AVOutputFormat *fmt;
  EncodedFrame * encodedFrame;
  
  int open_video();
  AVStream *add_video_stream(int width, int height);
public:
  VideoWriter();
  int init(std::string filename, int width, int height);
  int write(RawFrame * rawFrame);
  ~VideoWriter();
};

#endif /* VIDEOWRITER_H_ */
