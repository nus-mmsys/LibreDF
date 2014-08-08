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

#ifndef FITLER_H_
#define FITLER_H_

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Bus.h"

using namespace std;

class Port;

/*!
 * \enum FilterStatus
 * Status of a filter.
 */
enum FilterStatus {
	FILTER_SUCCESS, /**< Filter processed successfully. */
	FILTER_ERROR, /**< An error occurred while processing. */
	FILTER_FINISHED, /**< Filter is done generating more data. Used in data sources. */
	FILTER_WAIT_FOR_INPUT
};

/*!
 * \class Filter
 * A single filter in a pipeline.
 * Can be connected to multiple filters, and receive data from multiple filters.
 */
class Filter {
private:
	int linked;
	int inputFed;
	Bus * bus;
protected:

	vector<Port*> inputPorts;
	vector<Port*> outputPorts;

	/*!
	 * Process the input data using the filter.
	 */
	virtual FilterStatus process() = 0;

	/*!
	 * Perform initialization of the filter.
	 * To be overridden in subclasses to allow initialization of specific filter values.
	 */
	virtual FilterStatus init() {
		return FILTER_SUCCESS;
	}

	/*!
	 * \param name
	 *   The name of the filter.
	 */
	Filter(const string & name);

public:

	/*!
	 * Set a property of the filter.
	 *
	 * \param key
	 *   The property name.
	 * \param val
	 *   The property value.
	 */
	void setProp(const string & key, const string & val);

	/*!
	 * Get the value of a filter property.
	 *
	 * \param key
	 *   The property name.
	 */
	string getProp(const string & key);

	/*!
	 * Connect this filter to another filter in the pipeline.
	 * Use Pipeline::connect instead of this.
	 *
	 * \param f
	 *   The filter to connect to.
	 */
	void connectFilter(Filter * f);

	/*!
	 * Execute the processing of this filter.
	 * In case more information is needed, wait until it is provided.
	 *
	 * \return The new status of the filter.
	 */
	FilterStatus executeFilter();

	/*!
	 * Initialize this filter.
	 * Should be called before the first call to executeFilter(), after all properties for the filter are set.
	 */
	void initializeFilter();

	void increaseLinked() {
		linked++;
	}

	int inputPortNum() {
		return inputPorts.size();
	}

	int outputPortNum() {
		return outputPorts.size();
	}

	void setBus(Bus * b) {bus = b;}

	virtual ~Filter();
};

#endif /* FITLER_H_ */
