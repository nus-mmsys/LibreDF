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

#ifndef NUMBERGENERATORFILTER_H_
#define NUMBERGENERATORFILTER_H_

#include "include/Filter.h"

#include <string>
#include <unistd.h>

using namespace std;

class NumberGeneratorFilter: public Filter {

private:

	OutputPort<int> * outputInt;
	OutputPort<string> * outputString;

public:

	NumberGeneratorFilter(const string & name) :
			Filter(name) {
		outputInt = new OutputPort<int>("numbergenerator, output 1, int", this);
		outputString = new OutputPort<string>(
				"numbergenerator, output 2, string", this);

		outputPorts.push_back(outputInt);
		outputPorts.push_back(outputString);
	}


	FilterStatus process() {
		static int number = 1;

		//int * n = new int;
		int num = number;
		//BufferNode<int> bn(&n);

		//string * numstr = new string;
		string numstr = std::to_string(number);
		//BufferNode<string> bns(&numstr);

		outputInt->produce(&num);
		outputString->produce(&numstr);

		//This function must run on a separate therad.
		outputInt->process();
		//This function must run on a separate therad.
		outputString->process();

		number++;

		//sleep(1000);
		return FILTER_SUCCESS;
	}

	~NumberGeneratorFilter() {
		delete outputInt;
		delete outputString;
	}

};

#endif /* NUMBERGENERATORFILTER_H_ */
