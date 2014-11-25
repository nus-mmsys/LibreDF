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

#include "filters/libav/VideoMuxerFilter.h"

FilterRegister<VideoMuxerFilter> VideoMuxerFilter::reg("video_muxer");

VideoMuxerFilter::VideoMuxerFilter(string name) :
Filter(name) {
  
  inputPortEncodedFrame = createInputPort<EncodedFrame>("EncodedFrame input");
  
  videoMuxer = new VideoMuxer();
  
}

void VideoMuxerFilter::init() {
  
  Attribute * attr;
  
  string output_video;
  int width, height, bitrate, framerate;
  
  
  inputPortEncodedFrame->lockAttr();
  attr = inputPortEncodedFrame->getAttr();
  output_video = attr->getProp("output_video");
  width =  stoi(attr->getProp("width"));
  height = stoi(attr->getProp("height"));
  bitrate = stoi(attr->getProp("bitrate"));
  framerate = stoi(attr->getProp("framerate"));
  inputPortEncodedFrame->unlockAttr();
  
  videoMuxer->init(output_video, width, height, bitrate, framerate);
  
}

void  VideoMuxerFilter::run() {
  
  inputPortEncodedFrame->lock();
  
  if (inputPortEncodedFrame->getStatus() == SampleStatus::EOS) {
    status = FilterStatus::EOS; 
    inputPortEncodedFrame->unlock();
    return;
  }
  
  EncodedFrame * inFrame = (EncodedFrame*) inputPortEncodedFrame->get();
  videoMuxer->mux(inFrame);
  
  inputPortEncodedFrame->unlock();
  
}

VideoMuxerFilter::~VideoMuxerFilter() {
  destroyPort(inputPortEncodedFrame);
  delete videoMuxer;
}