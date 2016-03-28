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

#include "filters/libav/VideoDisplayFilter.h"

using namespace tmf;
using namespace std;

FilterRegister<VideoDisplayFilter> VideoDisplayFilter::reg("video_display");

VideoDisplayFilter::VideoDisplayFilter(string name) :
Filter(name) {
  
  inputPortRawFrame = createInputPort<RawFrame>("RawFrame input");
  
  videoDisplay = new VideoDisplay();
}

void VideoDisplayFilter::init() {
  
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
void VideoDisplayFilter::run() {
  
  int ret;
  
  inputPortRawFrame->lock();
  
  if (inputPortRawFrame->getStatus() == SampleStatus::EOS) {
    status = FilterStatus::EOS; 
    inputPortRawFrame->unlock();
    return;
  }
  
  RawFrame * inFrame = inputPortRawFrame->get();
  
  ret = videoDisplay->display(inFrame);
  
  if (ret < 0) {
    sendEvent(Event(EventType::EOS, std::string("")));
  }
  
  inputPortRawFrame->unlock();
  
}

VideoDisplayFilter::~VideoDisplayFilter() {
  
  destroyPort(inputPortRawFrame);
  delete videoDisplay;
}
