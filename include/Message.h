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

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <map>

using namespace std;


enum MessageError {
	MSG_OK, MSG_NOT_FOUND
};

class Message {
private:
	map<string, string> props;/**< The map of properties for the filter. */


	string getProp(const string & key) {
		std::map<string, string>::iterator it;

		it = props.find(key);

		if (it == props.end())
			return "";

		return props[key];
	}

public:

	void setProp(const string & key, const string & val) {
		props.insert(std::make_pair(key, val));
	}


	MessageError getPropInt(const string & key, int & val) {
		string valstr = getProp(key);

		if (valstr == "")
			return MSG_NOT_FOUND;

		val = std::stoi(valstr);
		return MSG_OK;
	}

	MessageError getPropString(const string & key, string & val) {
		string valstr = getProp(key);

		if (valstr == "")
			return MSG_NOT_FOUND;

		val = valstr;
		return MSG_OK;
	}

	void setPropInt(const string & key, const int & val) {
		setProp(key, std::to_string(val));
	}

};

#endif /* MESSAGE_H_ */
