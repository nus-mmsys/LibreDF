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

#include "filters/Add2Filter.h"
#include "filters/AdditionFilter.h"
#include "filters/DuplicateFilter.h"
#include "filters/Multiply2Filter.h"
#include "filters/NumberGeneratorFilter.h"
#include "filters/VideoDecoderFilter.h"
#include "filters/ImageWriterFilter.h"
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
	IMAGE_WRITER_FILTER

};

/*!
 * \class FilterFactory
 * Factory for creating filter objects.
 */
class TMF {
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
		if (name.empty())
			return NULL;

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

		case IMAGE_WRITER_FILTER:
			return new ImageWriterFilter(name);
		}

		return NULL;
	}

	Pipeline * createPipeline(const string & name) {
		return new Pipeline(name);
	}

	void destroyPipeline(Pipeline * pipe) {
		delete pipe;
	}
};

#endif /* FILTERFACTORY_H_ */
