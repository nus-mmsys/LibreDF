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

#include "filters/libav/tools/ImageWriter.h"

ImageWriter::ImageWriter(int width, int height, AVPixelFormat format) {
  
  int numBytes;
  buffer = 0;
  path = "./";
  this->width = width;
  this->height = height;
  this->format = format;
  //frameNumber = 0;
  
  // Allocate an AVFrame structure
  pFrameRGB = avcodec_alloc_frame();
  
  if (pFrameRGB == NULL) {
    std::cerr << "Cannot allocate frame." << endl;
    
  }
  
  // Determine required buffer size and allocate buffer
  numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, width, height);
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
  avpicture_fill((AVPicture *) pFrameRGB, buffer, AV_PIX_FMT_RGB24, width,
		 height);
  
}

void ImageWriter::writeImage(RawFrame * frame) {
  
  AVFrame * pFrame = frame->getFrame();
  
  sws_scale(sws_ctx, (uint8_t const * const *) pFrame->data,
	    pFrame->linesize, 0, height, pFrameRGB->data,
	    pFrameRGB->linesize);
  
  FILE *pFile;
  char szFilename[128];
  int y;
  
  // Open file
  sprintf(szFilename, "%s/frame%d.ppm", path.c_str(), frame->getNumber());
  pFile = fopen(szFilename, "wb");
  if (pFile == NULL)
    return;
  
  // Write header
  fprintf(pFile, "P6\n%d %d\n255\n", width, height);
  
  // Write pixel data
  for (y = 0; y < height; y++)
    fwrite(pFrameRGB->data[0] + y * pFrameRGB->linesize[0], 1,
	   width * 3, pFile);
    
    // Close file
    fclose(pFile);
  
}

void ImageWriter::setPath(string path) {
  this->path = path;
}

ImageWriter::~ImageWriter() {
  // Free the RGB image
  if(buffer) {	
    av_free(buffer);
  }
  if(pFrameRGB) {
    av_free(pFrameRGB);
  }
}

