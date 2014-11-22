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

#ifndef IMAGESCALER_H_
#define IMAGESCALER_H_

#include "types/RawFrame.h"

#ifdef __cplusplus
extern "C" {
  #endif
  #include <libavcodec/avcodec.h>
  #include <libavformat/avformat.h>
  #include <libswscale/swscale.h>
  #ifdef __cplusplus
}
#endif

class ImageScaler {
  
private:
  
  struct SwsContext *sws_ctx;
  int srcWidth;
  int srcHeight;
  AVPixelFormat srcFmt;
  int dstWidth;
  int dstHeight;
  AVPixelFormat dstFmt;
  
public:
  ImageScaler(int srcWidth, int srcHeight, AVPixelFormat srcFmt, int dstWidth, int dstHeight, AVPixelFormat dstFmt) {
    
    this->srcWidth = srcWidth;
    this->srcHeight = srcHeight;
    this->srcFmt = srcFmt;
    this->dstWidth = dstWidth;
    this->dstHeight = dstHeight;
    this->dstFmt = dstFmt;
    
    sws_ctx = sws_getContext(srcWidth, srcHeight, srcFmt, dstWidth, dstHeight, dstFmt, SWS_BILINEAR, NULL, NULL, NULL);
    
  }
  
  void fill(RawFrame * frame) {
   
    uint8_t * buffer = frame->getBuffer();
        
    if(buffer != 0)
      return;

    AVFrame * data = frame->getFrame();
    // Determine required buffer size and allocate buffer
    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, dstWidth, dstHeight);
    buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    // Assign appropriate parts of buffer to image planes in pFrameRGB
    // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
    // of AVPicture
    avpicture_fill((AVPicture *) data, buffer, dstFmt, dstWidth,
		  dstHeight);
  }
  
  int scale(RawFrame * inFrame, RawFrame * outFrame) {
    
    AVFrame * inAvFrame = inFrame->getFrame();
    AVFrame * outAvFrame = outFrame->getFrame();
   
    
    outFrame->setNumber(inFrame->getNumber());
    
    fill(outFrame);
    
    //avcodec_get_frame_defaults(outAvFrame);
    return sws_scale(sws_ctx, (uint8_t const * const *) inAvFrame->data,
		     inAvFrame->linesize, 0, srcHeight, outAvFrame->data,
		     outAvFrame->linesize);
    
  }
};

#endif /* IMAGESCALER_H_ */
