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

#ifndef PIPELINEDATA_H_
#define PIPELINEDATA_H_

using namespace std;

/*!
 * \class Buffer
 *
 * Buffer is a circular list of data.
 * Buffer is used in output ports.
 */

template<class Type>
class Buffer {

private:

	Type * buf; /**< the list of data */
	int curIndex; /**< the current index in the buffer */
	int size; /**< The size of the buffer */

public:

	/*!
	 * Buffer constructor
	 *
	 * \param size the size of the buffer
	 */
	Buffer(int size) {
		buf = new Type[size];
		curIndex = -1;
		this->size = size;
	}

	/*!
	 * Insert an element into the buffer
	 *
	 * \param e the element to be inserted
	 */
	void insert(Type e) {
		curIndex = (curIndex + 1) % size;
		buf[curIndex] = e;
	}

	/*!
	 * Get the current node in the buffer
	 *
	 * \return the current element of the buffer
	 */
	Type getNode() {
		return buf[curIndex];
	}

	/*!
	 * Get the size of the buffer
	 *
	 * \return the size of the buffer
	 */
	int getSize() {
		return size;
	}

	/*!
	 * Get an element of the buffer by index
	 *
	 * \param i the number of the element
	 * \return the element number i
	 */
	Type getNode(int i) {
		if (i < 0 || i >= size)
			return 0;
		return buf[i];
	}

	/*!
	 * Get the next element of the buffer. Used when the node is a reference and the client
	 * of the buffer wants to initialize the node.
	 *
	 * \return the next element of the buffer
	 */
	Type getNextNode() {
		int nextIndex = (curIndex + 1) % size;
		return buf[nextIndex];
	}

	/*!
	 * Buffer destructor
	 *
	 */
	~Buffer() {
		delete buf;
	}
};

#endif /* PIPELINEDATA_H_ */
