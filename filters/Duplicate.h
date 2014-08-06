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

#ifndef DUPLICATEFILTER_H_
#define DUPLICATEFILTER_H_

#include "Filter.h"
#include <unistd.h>

struct DuplicateFilter: public Filter {

private:
	InputPort<string> * input;
	OutputPort<string> * output;

public:

	DuplicateFilter(const string & name) :
			Filter(name) {
		input = new InputPort<string>("duplicate, input 1, string", this);
		output = new OutputPort<string>("duplicate, output 1, string", this);

		inputPorts.push_back(input);
		outputPorts.push_back(output);
	}

	void init() {
	}

	FilterStatus process() {

		BufferNode<string> * bn;

		bn = input->read();

		string * inputData = bn->getData();

		//string * outputData = new string;
		string outputData = *inputData + *inputData;

		//bn.setData(&outputData);

		//output->produce(bn);

		//output->process();

		cout << "Duplicate= " << outputData << endl;
		//usleep(500);
		return FILTER_SUCCESS;
	}

	~DuplicateFilter() {
	}

};

#endif /* DUPLICATEFILTER_H_ */
