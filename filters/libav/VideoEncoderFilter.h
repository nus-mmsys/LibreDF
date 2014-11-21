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

#ifndef VIDEOENCODERFILTER_H_
#define VIDEOENCODERFILTER_H_

#include "core/Filter.h"
#include "core/Port.h"
#include "types/RawFrame.h"
#include "types/EncodedFrame.h"
#include "tools/VideoEncoder.h"

class VideoEncoderFilter : public Filter {
  
private:
  VideoEncoder * videoEncoder;
  
  InputPort<RawFrame> * inputPortRawFrame;
  OutputPort<EncodedFrame> * outputPortEncodedFrame;
public:
  VideoEncoderFilter(string name) :
  Filter(name) {
    
    inputPortRawFrame = new InputPort<RawFrame>("RawFrame input");
    outputPortEncodedFrame = new OutputPort<EncodedFrame>("EncodedFrame output");
    
    inputPorts.push_back(inputPortRawFrame);
    outputPorts.push_back(outputPortEncodedFrame);
    
    videoEncoder = new VideoEncoder();
    
  }
  
  virtual void init() {
    
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
  
  virtual void run() {
    
    
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
  
  virtual ~VideoEncoderFilter() {
    
    delete inputPortRawFrame;
    delete outputPortEncodedFrame;
    delete videoEncoder;
  }
  
  
};

#endif /* VIDEOENCODERFILTER_H_ */
