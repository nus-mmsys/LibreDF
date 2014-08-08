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

#ifndef ADDITIONFILTER_H_
#define ADDITIONFILTER_H_

#include "Filter.h"
#include <unistd.h>

struct AdditionFilter: public Filter {

private:
	InputPort<int> * input1;
	InputPort<int> * input2;
	OutputPort<int> * output;

public:

	AdditionFilter(const string & name) :
			Filter(name) {
		input1 = new InputPort<int>("addition, input 1, int", this);
		input2 = new InputPort<int>("addition, input 2, int", this);
		output = new OutputPort<int>("addition, output 1, int", this);

		inputPorts.push_back(input1);
		inputPorts.push_back(input2);
		outputPorts.push_back(output);
	}


	FilterStatus process() {

		BufferNode<int> * bn;

		bn = input1->read();

		int * inputData1 = bn->getData();

		bn = input2->read();

		int * inputData2 = bn->getData();

		//int * outputData = new int;
		int outputData = *inputData1 + *inputData2;

		//bn.setData(&outputData);

		//output->produce(bn);

		//output->process();

		cout << "Addition= " << outputData << endl;
		//usleep(100000);
		return FILTER_SUCCESS;

	}

	~AdditionFilter() {
	}

};

#endif /* ADDITIONFILTER_H_ */
