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

#include "tmffactory/tmf.h"

Filter * TMF::createFilter(const FilterType type, const string& name) const {
  if (name.empty()) {
    std::cerr << "Filter name cannot be empty." << endl;
    return 0;
  }
  
  const bool is_in = elements.find(name) != elements.end();
  if (is_in) {
    std::cerr << "Filter name cannot be repeated." << endl;
    return 0;
  }
  
  switch (type) {
    case FilterType::DOUBLEPRODUCER:
      return new DoubleProducerFilter(name);
      
    case FilterType::STRINGPRODUCER:
      return new StringProducerFilter(name);
      
    case FilterType::INTPRODUCER:
      return new IntProducerFilter(name);
      
    case FilterType::MULTIPLY2:
      return new Multiply2Filter(name);
      
    case FilterType::DUPLICATE:
      return new DuplicateFilter(name);
      
    case FilterType::ADD2:
      return new Add2Filter(name);
      
    case FilterType::ADDITION:
      return new AdditionFilter(name);
      
    case FilterType::VIDEO_READER:
      return new VideoReaderFilter(name);
      
    case FilterType::VIDEO_ENCODER:
      return new VideoEncoderFilter(name);
      
    case FilterType::VIDEO_WRITER:
      return new VideoWriterFilter(name);
      
    case FilterType::VIDEO_MUXER:
      return new VideoMuxerFilter(name);
      
    case FilterType::VIDEO_DISPLAY:
      return new VideoDisplayFilter(name);
      
    case FilterType::IMAGE_WRITER:
      return new ImageWriterFilter(name);
      
    case FilterType::IMAGE_SCALER:
      return new ImageScalerFilter(name);
    default:
      cout << "Filter not found.\n";
      return nullptr;
  }
  
  return NULL;
}



Pipeline * TMF::createPipeline(const string & name) {
  return new Pipeline(name);
}


void TMF::destroyPipeline(Pipeline * pipe) {
  if (pipe)
    delete pipe;
}
