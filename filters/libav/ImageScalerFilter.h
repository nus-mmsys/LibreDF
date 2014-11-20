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

#ifndef IMAGESCALERFILTER_H_
#define IMAGESCALERFILTER_H_

#include "tools/VideoScaler.h"
#include "core/Filter.h"
#include "core/Port.h"

#include <string>

using namespace std;

class ImageScalerFilter : public Filter {
  
private:
  
  VideoScaler * videoScaler;
  
  InputPort<RawFrame> * inputPortFrame;
  OutputPort<RawFrame> * outputPortFrame;
  
public:
  
  ImageScalerFilter(string name) : Filter(name) {
    
    inputPortFrame = new InputPort<RawFrame>("RawFrame input");
    
    outputPortFrame = new OutputPort<RawFrame>("RawFrame output");
    
    inputPorts.push_back(inputPortFrame);
    outputPorts.push_back(outputPortFrame);
    
    videoScaler = nullptr;
  }
  
  void init() {
    
    Attribute * attr;    
    
    //string width = getProp("width");
    
    int dstWidth = stoi(getProp("width"));
    int dstHeight = stoi(getProp("height"));
    
    int srcWidth, srcHeight, srcFormatInt;
    
    inputPortFrame->lockAttr();
    attr = inputPortFrame->getAttr();
    srcWidth = stoi(attr->getProp("width"));
    srcHeight = stoi(attr->getProp("height"));
    srcFormatInt = stoi(attr->getProp("format"));
    inputPortFrame->unlockAttr();
    
    AVPixelFormat srcFormat = static_cast<AVPixelFormat>(srcFormatInt);
    
    videoScaler = new VideoScaler(srcWidth, srcHeight, srcFormat, dstWidth, dstHeight, srcFormat);
    
    //TODO FIXME
    // It is not good to get the buffer at this level.
    MediaSample<RawFrame> ** buf =  outputPortFrame->getSamples();
    for (int i=0; i<outputPortFrame->getBufferSize(); i++) {
      RawFrame * frame = buf[i]->get();
      frame->fill(dstWidth, dstHeight, srcFormat);
    }
    
    outputPortFrame->lockAttr();
    attr = outputPortFrame->getAttr();
    attr->setProp<int>("width", dstWidth);
    attr->setProp<int>("height", dstHeight);
    attr->setProp<int>("format", srcFormatInt);
    outputPortFrame->unlockAttr();
    
  }
  
  void run() {
    
    inputPortFrame->lock();
    
    if (inputPortFrame->getStatus() == SampleStatus::EOS) {
      status = FilterStatus::EOS; 
      inputPortFrame->unlock();
      
      outputPortFrame->lock();
      outputPortFrame->setStatus(SampleStatus::EOS);
      outputPortFrame->unlock();
      
      return;
    }
    
    RawFrame * inFrame = inputPortFrame->get();
    
    outputPortFrame->lock();
    RawFrame * outFrame = outputPortFrame->get();
    
    videoScaler->scale(inFrame, outFrame);
    
    inputPortFrame->unlock();
    
    outputPortFrame->unlock();
  }
  
  
  ~ImageScalerFilter() {
    delete inputPortFrame;
    delete outputPortFrame;
    delete videoScaler;
  }
  
};

#endif /* IMAGESCALERFILTER_H_ */
