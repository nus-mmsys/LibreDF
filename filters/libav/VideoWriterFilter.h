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

#ifndef VIDEOWRITERFILTER_H_
#define VIDEOWRITERFILTER_H_

#include "core/Filter.h"
#include "core/Port.h"
#include "types/RawFrame.h"
#include "tools/VideoWriter.h"

class VideoWriterFilter : public Filter {
private:
  VideoWriter * videoWriter;
  
  InputPort<RawFrame> * inputPortRawFrame;
public:
  
  VideoWriterFilter(string name) :
  Filter(name) {
    
    inputPortRawFrame = new InputPort<RawFrame>(
      "videoMuxer, input 1, RawFrame");
    
    inputPorts.push_back(inputPortRawFrame);
    
    videoWriter = new VideoWriter();
    
  }
  
  virtual void init() {
    
    Attribute * attr;
    
    int width, height;
    
    string output_video = getProp("output_video");
    
    inputPortRawFrame->lockAttr();
    attr = inputPortRawFrame->getAttr();
    width = stoi(attr->getProp("width"));
    height = stoi(attr->getProp("height"));
    inputPortRawFrame->unlockAttr();
    
    videoWriter->init(output_video, width, height);
  }
  virtual void run() {
    
    inputPortRawFrame->lock();
    
    if (inputPortRawFrame->getStatus() == SampleStatus::EOS) {
      status = FilterStatus::EOS; 
      inputPortRawFrame->unlock();
      return;
    }
    
    RawFrame * inFrame = inputPortRawFrame->get();
    
    videoWriter->write(inFrame);
    
    inputPortRawFrame->unlock();
    
  }
  
  virtual ~VideoWriterFilter() {
    delete videoWriter;
    delete inputPortRawFrame;
  }
  
  
};

#endif /* VIDEOWRITERFILTER_H_ */
