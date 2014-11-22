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

#ifndef IMAGEWRITERFILTER_H_
#define IMAGEWRITERFILTER_H_

#include "core/Port.h"
#include "core/Filter.h"
#include "tools/ImageWriter.h"

class ImageWriterFilter: public Filter {
  
private:
  
  ImageWriter * imageWriter;
  InputPort<RawFrame> * inputFrame;
  
public:
  ImageWriterFilter(string name) :
  Filter(name) {
    
    inputFrame = new InputPort<RawFrame>("RawFrame input");
    
    inputPorts.push_back(inputFrame);
    
    imageWriter = nullptr;
  }
  
  virtual void init() {
    
    int srcWidth, srcHeight, srcFormatInt;
    
    inputFrame->lockAttr();
    
    Attribute * attr = inputFrame->getAttr();
    
    srcWidth = stoi(attr->getProp("width"));
    
    srcHeight = stoi(attr->getProp("height"));
    
    srcFormatInt = stoi(attr->getProp("format"));
    
    inputFrame->unlockAttr();
    
    AVPixelFormat srcFormat = static_cast<AVPixelFormat>(srcFormatInt);
    
    imageWriter = new ImageWriter{srcWidth, srcHeight, srcFormat};
    imageWriter->setPath(getProp("output_path"));
    
  }
  
  virtual void run() {
    
    inputFrame->lock();	
    
    if (inputFrame->getStatus() == SampleStatus::EOS) {
      status = FilterStatus::EOS; 
      inputFrame->unlock();
      return;
    }
    
    RawFrame * data = inputFrame->get();
    imageWriter->writeImage(data);
    
    inputFrame->unlock();
  }
  
  virtual ~ImageWriterFilter() {
      delete inputFrame;
      delete imageWriter;
  }
  
  
};

#endif /* IMAGEWRITERFILTER_H_ */
