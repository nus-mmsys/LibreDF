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

#include "filters/libav/VideoReaderFilter.h"

VideoReaderFilter::VideoReaderFilter(string name) :
Filter(name) {
  
  outputFrame = createOutputPort<RawFrame>("RawFrame output");
  
  videoReader = nullptr;
}

void  VideoReaderFilter::init() {
  
  string inputVideo = getProp("input_video");
  string videoFromat = getProp("video_format");
  string inputFormat = getProp("input_format");
  
  videoReader = new VideoReader(inputVideo);
  videoReader->setVideoFromat(videoFromat);
  videoReader->setInputFormat(inputFormat);
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
  
  delete outputFrame;
  delete videoReader;
}