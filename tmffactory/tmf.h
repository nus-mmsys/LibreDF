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

#ifndef FILTERFACTORY_H_
#define FILTERFACTORY_H_

#include <string>
#include <set>

#include "filters/basic/Add2Filter.h"
#include "filters/basic/AdditionFilter.h"
#include "filters/basic/DuplicateFilter.h"
#include "filters/basic/Multiply2Filter.h"
#include "filters/basic/DoubleProducerFilter.h"
#include "filters/basic/ConsumerFilter.h"
#include "filters/basic/StringProducerFilter.h"
#include "filters/basic/IntProducerFilter.h"

#include "filters/libav/VideoReaderFilter.h"
#include "filters/libav/VideoEncoderFilter.h"
#include "filters/libav/VideoWriterFilter.h"
#include "filters/libav/VideoMuxerFilter.h"
#include "filters/libav/ImageWriterFilter.h"
#include "filters/libav/ImageScalerFilter.h"
#include "filters/libav/VideoDisplayFilter.h"

#include "core/Pipeline.h"

using namespace std;

/*!
 * \enum FiltersType
 *
 * The type of a filter.
 */
enum class FilterType {
  ADD2,
  ADDITION,
  DUPLICATE,
  MULTIPLY2,
  DOUBLEPRODUCER,
  INTPRODUCER,
  STRINGPRODUCER,
  CONSUMER,
  VIDEO_READER,
  VIDEO_WRITER,
  VIDEO_ENCODER,
  VIDEO_MUXER,
  VIDEO_DISPLAY,
  IMAGE_SCALER,
  IMAGE_WRITER
  
};

/*!
 * \class TMF
 * Factory for creating filter objects and pipeline.
 */
class TMF {
private:
  set<string> elements;
public:
  /*!
   * Create a filter of a specific type.
   *
   * \param type
   *   The type of filter to create.
   * \param name
   *   The name of the new filter.
   *
   * \return the created filter.
   */
  Filter * createFilter(const FilterType type, const string& name) const;
  
  
  template <typename T>
  Filter * createFilter(const FilterType type, const string& name) const {
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
      
      case FilterType::CONSUMER:
	return new ConsumerFilter<T>(name);
	
      default:
	cout << "Filter not found.\n";
	return nullptr;
    }
    return nullptr;
  }
  
  /*!
   * Create a pipeline.
   *
   * \param name
   *   The name of the pipeline.
   *
   * \return the created pipelines.
   */
  Pipeline * createPipeline(const string & name);
  
  /*!
   * Destroy the pipeline.
   *
   * \param pipe
   *   The reference to the pipeline.
   *
   */
  void destroyPipeline(Pipeline * pipe);
};

#endif /* FILTERFACTORY_H_ */