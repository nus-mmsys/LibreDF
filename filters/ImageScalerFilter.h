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

#include <string>


class ImageScalerFilter : public Filter {
  
private:
  
  VideoScaler * videoScaler;
  
  InputPort<RawFrame> * inputPortFrame;
  OutputPort<RawFrame> * outputPortFrame;
  
public:
  
  ImageScalerFilter(string name) : Filter(name) {
    
    inputPortFrame = new InputPort<RawFrame>("imageScaler, input, Frame");
    
    outputPortFrame = new OutputPort<RawFrame>("imageScaler, output, Frame");
    
    inputPorts.push_back(inputPortFrame);
    outputPorts.push_back(outputPortFrame);
    
    videoScaler = 0;
  }
  
  FilterStatus init() {
    
    MessageError err;
    FilterStatus status = FILTER_SUCCESS;
    
    string width = getProp("width");
    string height = getProp("height");
    
    int dstWidth = std::stoi(width);
    int dstHeight = std::stoi(height);
    
    int srcWidth, srcHeight, srcFormatInt;
    
    err = inMsg->getPropInt("width", srcWidth);
    //if (err == MSG_NOT_FOUND)
    //  return FILTER_WAIT_FOR_INPUT;
    
    err = inMsg->getPropInt("height", srcHeight);
    //if (err == MSG_NOT_FOUND)
    //  return FILTER_WAIT_FOR_INPUT;
    
    err = inMsg->getPropInt("format", srcFormatInt);
    //if (err == MSG_NOT_FOUND)
    //  return FILTER_WAIT_FOR_INPUT;
    
    AVPixelFormat srcFormat = static_cast<AVPixelFormat>(srcFormatInt);
    
    
    
    videoScaler = new VideoScaler(srcWidth, srcHeight, srcFormat, dstWidth, dstHeight, srcFormat);
    
    //TODO FIXME
    //for (int i=0; i<outputPortFrame->getBuffer()->getSize(); i++) {
    //	RawFrame * frame = outputPortFrame->getBuffer()->getNode(i);
    //	frame->fill(dstWidth, dstHeight, srcFormat);
    //}
    
    outMsg->setProp("width", width);
    outMsg->setProp("height", height);
    outMsg->setPropInt("format", srcFormatInt);
    
    return status;
    
  }
  
  FilterStatus run() {
    FilterStatus status = FILTER_SUCCESS;
    
    inputPortFrame->lock();
    RawFrame * inFrame = inputPortFrame->get();
    
    outputPortFrame->lock();
    RawFrame * outFrame = outputPortFrame->get();
    
    videoScaler->scale(inFrame, outFrame);
    
    inputPortFrame->unlock();
    
    outputPortFrame->unlock();
    
    return status;
  }
  
  
  ~ImageScalerFilter() {
    delete inputPortFrame;
    delete outputPortFrame;
  }
  
};


#endif /* IMAGESCALERFILTER_H_ */
