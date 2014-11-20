/*
 * VideoWriterFilter.h
 *
 *  Created on: Aug 18, 2014
 *      Author: arash
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
    
    int width, height;
    //int bitrate;
    
    string output_video = getProp("output_video");
    
    /*
     *  err = inMsg->getPropInt("width", width);
     *  //if (err == MSG_NOT_FOUND)
     *  //	return FILTER_WAIT_FOR_INPUT;
     *  
     *  err = inMsg->getPropInt("height", height);
     *  //if (err == MSG_NOT_FOUND)
     *  //	return FILTER_WAIT_FOR_INPUT;
     */
    
    //bitrate = 400000;
    
    videoWriter->init(output_video, width, height);
    
  }
  virtual void run() {
    
    inputPortRawFrame->lock();
    
    RawFrame * inFrame = inputPortRawFrame->get();
    
    videoWriter->write(inFrame);
    
    inputPortRawFrame->unlock();
    
  }
  
  virtual ~VideoWriterFilter() {
    if (videoWriter)
      delete videoWriter;
    if (inputPortRawFrame)
      delete inputPortRawFrame;
  }
  
  
};

#endif /* VIDEOWRITERFILTER_H_ */
