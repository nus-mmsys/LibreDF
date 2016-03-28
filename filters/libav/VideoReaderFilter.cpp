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

#include "filters/libav/VideoReaderFilter.h"

using namespace tmf;
using namespace std;

FilterRegister<VideoReaderFilter> VideoReaderFilter::reg("video_reader");

VideoReaderFilter::VideoReaderFilter(string name) :
Filter(name) {
  
  outputFrame = createOutputPort<RawFrame>("RawFrame output");
  
  videoReader = nullptr;
}

void  VideoReaderFilter::init() {
  
  string inputVideo = getProp("input_video");
  string videoFromat = getProp("video_format");
  string inputFormat = getProp("input_format");
  string framerate = getProp("framerate");
  
  videoReader = new VideoReader(inputVideo);
  videoReader->setVideoFromat(videoFromat);
  videoReader->setInputFormat(inputFormat);
  videoReader->setFramerate(framerate);
  videoReader->init();
  
  videoReader->dump();
  
  outputFrame->lockAttr();
  
  Attribute * attr = outputFrame->getAttr();
  
  attr->setProp<int>("width", videoReader->getWidth());
  attr->setProp<int>("height", videoReader->getHeight());
  attr->setProp<int>("format",
		     static_cast<int>(videoReader->getPixFormat()));
  
  outputFrame->unlockAttr();
  
}

void  VideoReaderFilter::run() {
  
  outputFrame->lock();
  RawFrame * data = outputFrame->get();
  
  int ret = videoReader->readFrame(data);
  
  if (ret == -1) {
    outputFrame->setStatus(SampleStatus::EOS);
    status = FilterStatus::EOS;
  }
  outputFrame->unlock();
}

VideoReaderFilter::~VideoReaderFilter() {
  
  destroyPort(outputFrame);
  delete videoReader;
}
