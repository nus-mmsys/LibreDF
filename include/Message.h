/*
 * Message.h
 *
 *  Created on: Aug 11, 2014
 *      Author: arash
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
