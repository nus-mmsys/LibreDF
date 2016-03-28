/*
 * 
 *  Tiny Multimedia Framework (tmf)
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This file is part of tmf.
 *
 *  tmf is free software; you can redistribute it and/or 
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  tmf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with tmf.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "filters/libav/ImageScalerFilter.h"

using namespace tmf;
using namespace std;

FilterRegister<ImageScalerFilter> ImageScalerFilter::reg("image_scaler");

ImageScalerFilter::ImageScalerFilter(string name) : Filter(name) {
  
  inputPortFrame = createInputPort<RawFrame>("RawFrame input");
  
  outputPortFrame = createOutputPort<RawFrame>("RawFrame output");
  
  imageScaler = nullptr;
}

void ImageScalerFilter::init() {
  
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

void ImageScalerFilter::run() {
  
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


ImageScalerFilter::~ImageScalerFilter() {
  destroyPort(inputPortFrame);
  destroyPort(outputPortFrame);
  delete imageScaler;
}
