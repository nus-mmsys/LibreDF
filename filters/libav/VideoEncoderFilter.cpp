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

#include "filters/libav/VideoEncoderFilter.h"

using namespace tmf;
using namespace std;

FilterRegister<VideoEncoderFilter> VideoEncoderFilter::reg("video_encoder");

VideoEncoderFilter::VideoEncoderFilter(string name) :
Filter(name) {
  
  inputPortRawFrame = createInputPort<RawFrame>("RawFrame input");
  outputPortEncodedFrame = createOutputPort<EncodedFrame>("EncodedFrame output");
  
  videoEncoder = new VideoEncoder();
  
}

void VideoEncoderFilter::init() {
  
  Attribute * attr;
  
  int width, height, bitrate, framerate;
  
  string output_video = getProp("output_video");
  
  bitrate = std::stoi(getProp("bitrate"));
  
  framerate = std::stoi(getProp("framerate"));
  
  inputPortRawFrame->lockAttr();
  attr = inputPortRawFrame->getAttr();
  width = stoi(attr->getProp("width"));
  height = stoi(attr->getProp("height"));
  inputPortRawFrame->unlockAttr();
  
  videoEncoder->init(output_video, width, height, bitrate, framerate);
  
  outputPortEncodedFrame->lockAttr();
  attr = outputPortEncodedFrame->getAttr();
  attr->setProp("width", width);
  attr->setProp("height", height);
  attr->setProp("bitrate", bitrate);
  attr->setProp("framerate", framerate);
  attr->setProp("output_video", output_video);
  outputPortEncodedFrame->unlockAttr();
  
}

void VideoEncoderFilter::run() {
  
  
  inputPortRawFrame->lock();
  
  if (inputPortRawFrame->getStatus() == SampleStatus::EOS) {
    status = FilterStatus::EOS; 
    inputPortRawFrame->unlock();
    
    outputPortEncodedFrame->lock();
    outputPortEncodedFrame->setStatus(SampleStatus::EOS);
    outputPortEncodedFrame->unlock();
    
    return;
  }
  
  RawFrame * inFrame = inputPortRawFrame->get();
  
  outputPortEncodedFrame->lock();
  
  EncodedFrame * outFrame = outputPortEncodedFrame->get();
  videoEncoder->encode(inFrame, outFrame);
  
  outputPortEncodedFrame->unlock();
  
  inputPortRawFrame->unlock();
  
}

VideoEncoderFilter::~VideoEncoderFilter() {
  
  destroyPort(inputPortRawFrame);
  destroyPort(outputPortEncodedFrame);
  delete videoEncoder;
}
