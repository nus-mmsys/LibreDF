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

#include "filters/Add2Filter.h"
#include "filters/AdditionFilter.h"
#include "filters/DuplicateFilter.h"
#include "filters/Multiply2Filter.h"
#include "filters/NumberGeneratorFilter.h"
#include "filters/VideoDecoderFilter.h"
#include "filters/VideoEncoderFilter.h"
#include "filters/VideoWriterFilter.h"
#include "filters/VideoMuxerFilter.h"
#include "filters/ImageWriterFilter.h"
#include "filters/ImageScalerFilter.h"
#include "filters/VideoDisplayFilter.h"
#include "Pipeline.h"

using namespace std;

/*!
 * \enum FiltersType
 *
 * The type of a filter.
 */
enum FiltersType {
	ADD2_FILTER,
	ADDITION_FILTER,
	DUPLICATE_FILTER,
	MULTIPLY2_FILTER,
	NUMBERGENERATOR_FILTER,
	VIDEO_DECODER_FILTER,
	VIDEO_WRITER_FILTER,
	VIDEO_ENCODER_FILTER,
	VIDEO_MUXER_FILTER,
	VIDEO_DISPLAY_FILTER,
	IMAGE_SCALER_FILTER,
	IMAGE_WRITER_FILTER

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
	Filter * createFilter(const FiltersType type, const string& name) const {
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
		case NUMBERGENERATOR_FILTER:
			return new NumberGeneratorFilter(name);

		case MULTIPLY2_FILTER:
			return new Multiply2Filter(name);

		case DUPLICATE_FILTER:
			return new DuplicateFilter(name);

		case ADD2_FILTER:
			return new Add2Filter(name);

		case ADDITION_FILTER:
			return new AdditionFilter(name);

		case VIDEO_DECODER_FILTER:
			return new VideoDecoderFilter(name);

		case VIDEO_ENCODER_FILTER:
			return new VideoEncoderFilter(name);

		case VIDEO_WRITER_FILTER:
			return new VideoWriterFilter(name);

		case VIDEO_MUXER_FILTER:
			return new VideoMuxerFilter(name);

		case VIDEO_DISPLAY_FILTER:
			return new VideoDisplayFilter(name);

		case IMAGE_WRITER_FILTER:
			return new ImageWriterFilter(name);

		case IMAGE_SCALER_FILTER:
			return new ImageScalerFilter(name);
		}

		return NULL;
	}

	/*!
	 * Create a pipeline.
	 *
	 * \param name
	 *   The name of the pipeline.
	 *
	 * \return the created pipelines.
	 */
	Pipeline * createPipeline(const string & name) {
		return new Pipeline(name);
	}

	/*!
	 * Destroy the pipeline.
	 *
	 * \param pipe
	 *   The reference to the pipeline.
	 *
	 */
	void destroyPipeline(Pipeline * pipe) {
		delete pipe;
	}
};

#endif /* FILTERFACTORY_H_ */
