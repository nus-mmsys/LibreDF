/*
 * VideoDisplayFilter.h
 *
 *  Created on: Aug 27, 2014
 *      Author: arash
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
