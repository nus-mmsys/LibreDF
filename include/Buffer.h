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
 * \enum DataStatus
 *
 * What the status of the data is. Used for describing corrupt or missing data.
 */
enum BufferNodeStatus {
	VALID, /**< The data is valid and can be used freely. */
	INVALID /**< The data is invalid (corrupt or missing). */
};

template<class Type>
class BufferNode {

private:
	Type * data;
	BufferNodeStatus status;

public:
	BufferNode() {
		setStatus(INVALID);
		setData(0);
	}

	BufferNode(Type * d) {
		setStatus(VALID);
		setData(d);
	}

	Type * getData() {
		return data;
	}
	void setData(Type * d) {
		if (data == 0)
			delete data;
		data = d;
	}

	void setStatus(BufferNodeStatus bns) {
		status = bns;
	}
	BufferNodeStatus getStatus() {
		return status;
	}

};

/*!
 * \class PipelineData
 *
 * Data that are being sent over the pipeline.
 * Subclasses describe the concrete data.
 */

template<class Type>
class Buffer {
private:
	//DataStatus status;	/**< The current status of the data. */

	BufferNode<Type> ** buf;
	int curIndex;
	int size;

public:
	Buffer(int size) {
		buf = new BufferNode<Type> * [size];
		curIndex = 0;
		this->size = size;

	}

	void insert(BufferNode<Type> * bn) {
		buf[curIndex] = bn;
		curIndex = (curIndex + 1) % size;
	}

	BufferNode<Type> * getLast() {
		int prevIndex = (curIndex - 1 + size) % size;
		if (buf[prevIndex]->getStatus() == VALID) {
			return buf[prevIndex];
		}
		return 0;
	}
};

#endif /* PIPELINEDATA_H_ */
