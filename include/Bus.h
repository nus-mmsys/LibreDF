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

#ifndef BUS_H_
#define BUS_H_

#include <set>

using namespace std;

class Bus {

private:
	map<string, string> props;/**< The map of properties for the filter. */
public:

	void setProp(const string & key, const string & val) {
		props.insert(std::make_pair(key, val));
	}

	string getProp(const string & key) {
		return props[key];
	}

};

#endif /* BUS_H_ */
