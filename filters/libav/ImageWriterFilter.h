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

#ifndef IMAGEWRITER_H_
#define IMAGEWRITER_H_

#include "core/Filter.h"
#include "tools/VideoFrameWriter.h"

class ImageWriterFilter: public Filter {
  
private:
  
  VideoFrameWriter * videoFrameWriter;
  InputPort<RawFrame> * inputFrame;
  
public:
  ImageWriterFilter(string name) :
  Filter(name) {
    
    inputFrame = new InputPort<RawFrame>("RawFrame input");
    
    inputPorts.push_back(inputFrame);
    
    videoFrameWriter = nullptr;
  }
  
  void init() {
    
    MessageError err;
    //string videoName = getProp("input_video");
    
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
    
    videoFrameWriter = new VideoFrameWriter(srcWidth, srcHeight, srcFormat);
    videoFrameWriter->setPath(getProp("output_path"));
    
  }
  
  void run() {
    
    inputFrame->lock();	
    RawFrame * data = inputFrame->get();
    videoFrameWriter->writeImage(data);
    inputFrame->unlock();
  }
  
  ~ImageWriterFilter() {
    delete inputFrame;
    delete videoFrameWriter;
  }
  
};

#endif /* IMAGEWRITER_H_ */
