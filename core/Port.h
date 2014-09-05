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
#include <thread>

#define TMF_BUFFER_SIZE 10

/*!
 * \class Port
 * Abstraction of a port in a filter.
 * A port can be either input port of output port.
 */
class Port {

private:
	string name; /**< The name of the port */
	int linked; /**< The number of port connected to this port */

protected:
	string type; /**< The type of the buffer of the port (note: the typeid is used to
	 retrieve the type of the buffer and the type name is not complete.
	 This is used when a filter wants to connect ports and needs to know
	 the type of the ports) */

	//Filter * owner; /**< The filter which owns this port */

	vector<Port*> nextPorts; /**< A list of the next ports. A subclass filter must add its filters to this list */

public:

	/*!
	 * Port constructor
	 *
	 * \param name The name of the filter.
	 * \param owner The owner of the filter
	 */
	Port(string name /*, Filter * owner*/) :
			name(name), linked(0), type("") {
		//this->owner = owner;
	}

	/*!
	 * Get the name of the port
	 *
	 * \return the name of the port.
	 */
	string getName() {
		return name;
	}

	/*!
	 * Get the number of the ports connected to this port
	 *
	 * \return the number of the port.
	 */
	int getLinked() {
		return linked;
	}

	/*!
	 * Increase the number of the ports linked to the port.
	 * (The filter uses this function when it connects two filters)
	 *
	 */
	void increaseLinked() {
		linked++;
	}

	/*!
	 * Get the type of the port
	 *
	 * \return the type of the port
	 */
	string getType() {
		return type;
	}

	/*!
	 * Get the owner of the port
	 *
	 * \return the owner of the filter
	 */
	//Filter * getOwner() {
	//	return owner;
	//}
	/*!
	 * Get next ports
	 *
	 * \return the next ports
	 */
	vector<Port*> & getNextPorts() {
		return nextPorts;
	}

	/*!
	 * Add next port to this port
	 *
	 * \param n next port to connect to
	 */
	void addNextPort(Port* n) {
		nextPorts.push_back(n);
		this->increaseLinked();
		n->increaseLinked();
		//n->owner->increaseLinked();
	}

	/*!
	 * Port descructor
	 *
	 */
	virtual ~Port() {
	}
};

/*!
 * \class InputPort
 *
 * InputPort class is a subclass of the Port class.
 * It is a class template and the type of the buffer of the port is a template.
 *
 */
template<class Type>
class InputPort: public Port {

private:
	Type * inBufNode; /**< Input buffer node */

public:

	/*!
	 * InputPort constructor
	 *
	 * \param name The name of the port
	 * \param owner The owner of the port
	 *
	 */
	InputPort(string name/*, Filter * owner*/) :
			Port(name/*, owner*/) {
		type = string((typeid(Type).name()));
		inBufNode = 0;

	}

	/*!
	 * Consume a data coming from
	 * The output port calls the consume of input port and owner of this port is executed
	 *
	 * \param bn the data to be consumed
	 *
	 */
	void consume(Type * bn) {
		inBufNode = bn;
		//owner->executeFilter();
	}

	/*!
	 * Read data from the port
	 *
	 * \return input buffer of the port
	 */
	Type * read() {
		return inBufNode;
	}

	/*!
	 * InputPort destructor
	 *
	 */
	~InputPort() {
	}

};

/*!
 * \class OutputPort
 *
 * OutputPort class is a subclass of the Port class.
 * It is a class template and the type of the buffer of the port is a template.
 *
 */
template<class Type>
class OutputPort: public Port {

private:
	Buffer<Type> * outBuf; /**< The output buffer of the port */

public:

	/*!
	 * OutputPort constructor
	 *
	 * \param name The name of the output port
	 * \param owner The owner of the port
	 *
	 */
	OutputPort(string name/*, Filter * owner*/) :
			Port(name/*, owner*/) {
		outBuf = new Buffer<Type>(TMF_BUFFER_SIZE);
		type = string(typeid(Type).name());
	}

	/*!
	 * Produce data
	 * This function produce data on the output buffer
	 *
	 * \param bn data to be produced
	 *
	 */
	void produce(Type * bn) {
		outBuf->insert(bn);
	}

	/*!
	 * Get buffer
	 *
	 * \return the output port buffer
	 */
	Buffer<Type> * getBuffer() {
		return outBuf;
	}

	/*!
	 * Process the port
	 * It calls consume function of the next ports and therefore executes the next filters
	 *
	 */

	//This function must run on a separate thread.
	void processThread(Filter *f) {
		Type * bn = outBuf->getNode();

		vector<Port*>::iterator it;
		for (it = nextPorts.begin(); it != nextPorts.end(); ++it) {
			((InputPort<Type>*) (*it))->consume(bn);
		}

		f->processNextFilters(this);
	}

	void process(Filter *f) {
		std::thread thr(&OutputPort<Type>::processThread, this, f);
		thr.join();
	}



	/*!
	 * OutputPort desctructor
	 *
	 */
	~OutputPort() {
		delete outBuf;
	}
};

#endif /* PORT_H_ */
