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

#include "filters/libav/ImageWriterFilter.h"

using namespace tmf;
using namespace std;

FilterRegister<ImageWriterFilter> ImageWriterFilter::reg("image_writer");

ImageWriterFilter::ImageWriterFilter(string name) : Filter(name) {
  
  inputFrame = createInputPort<RawFrame>("RawFrame input");
  
  imageWriter = nullptr;
}

void ImageWriterFilter::init() {
  
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

void ImageWriterFilter::run() {
  
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

ImageWriterFilter::~ImageWriterFilter() {
  destroyPort(inputFrame);
  delete imageWriter;
}

