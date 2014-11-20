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

#ifndef VIDEODISPLAYFILTER_H_
#define VIDEODISPLAYFILTER_H_

#include "core/Filter.h"
#include "core/Port.h"
#include "types/RawFrame.h"
#include "tools/VideoDisplay.h"

class VideoDisplayFilter : public Filter {
  
private:
  VideoDisplay * videoDisplay;
  
  InputPort<RawFrame> * inputPortRawFrame;
public:
  
  VideoDisplayFilter(string name) :
  Filter(name) {
    
    inputPortRawFrame = new InputPort<RawFrame>(
      "videoEncoder, input 1, RawFrame");
    
    inputPorts.push_back(inputPortRawFrame);
    
    videoDisplay = new VideoDisplay();
    
  }
  
  virtual void init() {
    
    Attribute * attr;
    
    int width, height, pixFmtInt;
    
    inputPortRawFrame->lockAttr();
    attr = inputPortRawFrame->getAttr();
    width = stoi(attr->getProp("width"));
    height = stoi(attr->getProp("height"));
    pixFmtInt = stoi(attr->getProp("format"));
    inputPortRawFrame->unlockAttr();
    
    AVPixelFormat pixFmt = static_cast<AVPixelFormat>(pixFmtInt);
    
    videoDisplay->init(width, height, pixFmt);
    
  }
  virtual void run() {
    
    inputPortRawFrame->lock();
    
    if (inputPortRawFrame->getStatus() == SampleStatus::EOS) {
      status = FilterStatus::EOS; 
      inputPortRawFrame->unlock();
      return;
    }
    
    RawFrame * inFrame = inputPortRawFrame->get();
    
    videoDisplay->display(inFrame);
    
    inputPortRawFrame->unlock();
    
  }
  
  virtual ~VideoDisplayFilter() {
    
    if (inputPortRawFrame)
      delete inputPortRawFrame;
    if (videoDisplay)
      delete videoDisplay;
  }
  
  
};

#endif /* VIDEODISPLAYFILTER_H_ */
