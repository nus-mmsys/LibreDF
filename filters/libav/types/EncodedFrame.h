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
  
private:
  AVPacket * data;
public:
  EncodedFrame() {
    data = (AVPacket *) av_malloc(sizeof(AVPacket));
  }
  AVPacket * getPacket() {
    
    return data;
  }
  
};

#endif /* ENCODEDFRAME_H_ */
