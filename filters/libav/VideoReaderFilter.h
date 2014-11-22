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

#ifndef VIDEOREADERFILTER_H_
#define VIDEOREADERFILTER_H_

#include "core/Filter.h"
#include "core/Port.h"
#include "tools/VideoReader.h"
#include "types/RawFrame.h"

class VideoReaderFilter: public Filter {
  
private:
  VideoReader * videoReader;
  
  OutputPort<RawFrame> * outputFrame;
public:
  VideoReaderFilter(string name) :
  Filter(name) {
    
    outputFrame = new OutputPort<RawFrame>("RawFrame output");
    
    outputPorts.push_back(outputFrame);
    
    videoReader = nullptr;
  }
  
  virtual void init() {
    
    string videoName = getProp("input_video");
    string videoFromat = getProp("video_format");
    string input_video = getProp("input_video");
    
    videoReader = new VideoReader(videoName);
    videoReader->setVideoFromat(videoFromat);
    videoReader->setInputFormat(input_video);
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
  
  virtual void run() {
    
    outputFrame->lock();
    RawFrame * data = outputFrame->get();
    
    int ret = videoReader->readFrame(data);
    
    if (ret == -1) {
      outputFrame->setStatus(SampleStatus::EOS);
      status = FilterStatus::EOS;
    }
    outputFrame->unlock();
  }
  
  virtual ~VideoReaderFilter() {
    
    delete outputFrame;
    delete videoReader;
  }
  
};

#endif /* VIDEOREADERFILTER_H_ */
