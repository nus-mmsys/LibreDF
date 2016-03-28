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

#include "filters/libav/tools/VideoReader.h"

VideoReader::VideoReader(string name) {
  
  this->name = name;
  pFormatCtx = 0;
  pCodecCtx = 0;
  optionsDict = 0;
  vstream_idx = -1;
  input_format = "";
  video_format = "";
  framerate = "";
  
  // Register all formats and codecs
  av_register_all();
  avcodec_register_all();
  avdevice_register_all();
  avformat_network_init();
}

void VideoReader::setVideoFromat(string vformat) {
  this->video_format = vformat;
  
  if (vformat != "") {
    video_fmt = av_find_input_format(vformat.c_str());
    if (video_fmt == NULL) {
      std::cerr << "Could not set video format: " << vformat << endl;
    }
  }
}

void VideoReader::setInputFormat(string iformat) {
  int ret;
  this->input_format = iformat;
  
  if (input_format != "") {
    ret = av_dict_set(&optionsDict, "input_format", input_format.c_str(), 0);
    if (ret < 0) {
      std::cerr << "Could not set input format: " << input_format << endl;
    }
  }
}

void VideoReader::setFramerate(string framerate) {
  int ret;
  this->framerate = framerate;
  if (framerate != "") {
    ret = av_dict_set(&optionsDict, "framerate", framerate.c_str(), 0);
    if (ret < 0) {
      std::cerr << "Could not set framerate: " << framerate << endl;
    }
  }
}

int VideoReader::init() {
  
  AVCodec *pCodec = 0;
  
  frameNumber = 0;
  int ret;
  
  unsigned int i;
  
  ret = avformat_open_input(&pFormatCtx, name.c_str(), video_fmt, optionsDict? &optionsDict : NULL);
  if (ret < 0) {
    std::cerr << "Cannot open the input file: " << name << endl;
    return -1; // Couldn't open file
  }
  
  // Retrieve stream information
  if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
    std::cerr << "Cannot find stream information: " << name << endl;
    return -1; // Couldn't find stream information
  }
  
  // Find the first video stream
  for (i = 0; i < pFormatCtx->nb_streams; i++)
    if (pFormatCtx->streams[i]->codec->codec_type
      == AVMEDIA_TYPE_VIDEO) {
      vstream_idx = i;
    break;
      }
      if (vstream_idx == -1) {
	std::cerr << "Cannot find a video stream: " << name << endl;
	return -1; // Didn't find a video stream
      }
      
      // Get a pointer to the codec context for the video stream
      pCodecCtx = pFormatCtx->streams[vstream_idx]->codec;
      
      // Find the decoder for the video stream
      pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
      if (pCodec == NULL) {
	std::cerr << "Unsupported codec: " << name << endl;
	return -1; // Codec not found
      }
      // Open codec
      if (avcodec_open2(pCodecCtx, pCodec, &optionsDict) < 0) {
	std::cerr << "Cannot open codec: " << name << endl;
	return -1; // Could not open codec
      }
      
      return 0;
      
}

void VideoReader::dump() {
  // Dump information about file onto standard error
  av_dump_format(pFormatCtx, 0, name.c_str(), 0);
}

int VideoReader::getWidth() {
  return pCodecCtx->width;
}

int VideoReader::getHeight() {
  return pCodecCtx->height;
}

AVPixelFormat VideoReader::getPixFormat() {
  return pCodecCtx->pix_fmt;
}

AVCodecContext * VideoReader::getCodecContext() {
  return pCodecCtx;
}

AVFormatContext * VideoReader::getFormatContext() {
  return pFormatCtx;
}

int VideoReader::readFrame(RawFrame * frame) {
  
  AVFrame * pFrame = frame->getFrame();
  AVPacket packet;
  int frameFinished;
  
  avcodec_get_frame_defaults(pFrame);
  
  // Read frames
  while (av_read_frame(pFormatCtx, &packet) >= 0) {
    
    // Is this a packet from the video stream?
    if (packet.stream_index == vstream_idx) {
      
      
      
      // Decode video frame
      avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished,
			    &packet);
      
      // Did we get a video frame?
      if (frameFinished) {
	
	av_free_packet(&packet);
	frame->setNumber(frameNumber);
	frameNumber++;
	return 0;
	
      }
    }
    
    // Free the packet that was allocated by av_read_frame
    av_free_packet(&packet);
  }
  
  
  
  // Flush decoder
  packet.data = NULL;
  packet.size = 0;
  avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
  
  // Did we get a video frame?
  if (frameFinished) {
    
    av_free_packet(&packet);
    frame->setNumber(frameNumber);
    frameNumber++;
    return 0;
    
  }
  
  return -1;
}


VideoReader::~VideoReader() {
  // Close the codec
  avcodec_close(pCodecCtx);
  
  // Close the video file
  avformat_close_input(&pFormatCtx);
}
