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

#ifndef PORT_H_
#define PORT_H_

#include "Buffer.h"
#include "Filter.h"
#include <typeinfo>

#define TMF_BUFFER_SIZE 10

class Port {

private:
	string name;
	int linked;

protected:
	string type;
	vector<Port*> nextPorts;
	Filter * owner;

public:
	Port(string name, Filter * owner) :
			name(name), linked(0), type("") {
		this->owner = owner;
	}

	string getName() {
		return name;
	}
	;
	int getLinked() {
		return linked;
	}
	void increaseLinked() {
		linked++;
	}
	string getType() {
		return type;
	}

	void addNextPort(Port* n) {
		nextPorts.push_back(n);
		this->increaseLinked();
		n->increaseLinked();
		n->owner->increaseLinked();
	}
};

template<class Type>
class InputPort: public Port {

private:
	BufferNode<Type> * inBufNode;

public:

	InputPort(string name, Filter * owner) :
			Port(name, owner) {
		type = string((typeid(Type).name()));
		inBufNode = 0;

	}

	void consume(BufferNode<Type> * bn) {
		inBufNode = bn;
		owner->executeFilter();
	}

	BufferNode<Type> * read() {
		return inBufNode;
	}

};

template<class Type>
class OutputPort: public Port {

private:
	Buffer<Type> * outBuf;

public:

	OutputPort(string name, Filter * owner) :
			Port(name, owner) {
		outBuf = new Buffer<Type>(TMF_BUFFER_SIZE);
		type = string(typeid(Type).name());
	}

	void produce(BufferNode<Type> * bn) {
		outBuf->insert(bn);
	}

	//This function must run on a separate thread.
	int process() {
		BufferNode<Type> * bn = outBuf->getLast();
		if (bn == NULL)
			return -1;

		vector<Port*>::iterator it;
		for (it = nextPorts.begin(); it != nextPorts.end(); ++it) {
			((InputPort<Type>*) (*it))->consume(bn);
		}
		return 0;
	}
};

#endif /* PORT_H_ */
