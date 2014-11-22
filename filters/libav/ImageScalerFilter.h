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

#include "tools/ImageScaler.h"
#include "core/Filter.h"
#include "core/Port.h"

#include <string>

using namespace std;

class ImageScalerFilter : public Filter {
  
private:
  
  ImageScaler * imageScaler;
  
  InputPort<RawFrame> * inputPortFrame;
  OutputPort<RawFrame> * outputPortFrame;
  
public:
  
  ImageScalerFilter(string name) : Filter(name) {
    
    inputPortFrame = new InputPort<RawFrame>("RawFrame input");
    
    outputPortFrame = new OutputPort<RawFrame>("RawFrame output");
    
    inputPorts.push_back(inputPortFrame);
    outputPorts.push_back(outputPortFrame);
    
    imageScaler = nullptr;
  }
  
  virtual void init() {
    
    Attribute * attr;    
    
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
    
    imageScaler = new ImageScaler(srcWidth, srcHeight, srcFormat, dstWidth, dstHeight, srcFormat);
    
    outputPortFrame->lockAttr();
    attr = outputPortFrame->getAttr();
    attr->setProp<int>("width", dstWidth);
    attr->setProp<int>("height", dstHeight);
    attr->setProp<int>("format", srcFormatInt);
    outputPortFrame->unlockAttr();
  }
  
  virtual void run() {
    
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
    
    imageScaler->scale(inFrame, outFrame);
    
    inputPortFrame->unlock();
    
    outputPortFrame->unlock();
  }
  
  
  virtual ~ImageScalerFilter() {
    delete inputPortFrame;
    delete outputPortFrame;
    delete imageScaler;
  }
  
};

#endif /* IMAGESCALERFILTER_H_ */
