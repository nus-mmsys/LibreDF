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

#ifndef VIDEOMUXERFILTER_H_
#define VIDEOMUXERFILTER_H_

#include "core/Filter.h"
#include "core/Port.h"
#include "types/EncodedFrame.h"
#include "tools/VideoMuxer.h"

class VideoMuxerFilter: public Filter {
private:
  VideoMuxer * videoMuxer;
  
  InputPort<EncodedFrame> * inputPortEncodedFrame;
public:
  
  VideoMuxerFilter(string name) :
  Filter(name) {
    
    inputPortEncodedFrame = new InputPort<EncodedFrame>("EncodedFrame input");
    
    inputPorts.push_back(inputPortEncodedFrame);
    
    videoMuxer = new VideoMuxer();
    
  }
  
  virtual void init() {
    
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
  
  virtual void run() {
    
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
  
  virtual ~VideoMuxerFilter() {
    delete inputPortEncodedFrame;
    delete videoMuxer;
  }
  
  
};

#endif /* VIDEOMUXERFILTER_H_ */
