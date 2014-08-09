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
//enum BufferNodeStatus {
//	VALID, /**< The data is valid and can be used freely. */
//	INVALID /**< The data is invalid (corrupt or missing). */
//};

/*
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
		//if (data == 0)
		//	delete data;
		setStatus(VALID);
		data = d;
	}

	void setStatus(BufferNodeStatus bns) {
		status = bns;
	}
	BufferNodeStatus getStatus() {
		return status;
	}

	~BufferNode() {

	}



};
*/

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

	Type * buf;
	int curIndex;
	int size;

public:
	Buffer(int size) {
		buf = new Type[size];
		curIndex = -1;
		this->size = size;
	}

	void insert(Type bn) {
		curIndex = (curIndex + 1) % size;
		buf[curIndex] = bn;
	}

	Type getNode() {
		return buf[curIndex];
	}

	int getSize() {return size;}

	Type getNode(int i) {
		return buf[i];
	}

	Type getNextNode() {
		int nextIndex = (curIndex + 1) % size;
		return buf[nextIndex];
	}

	~Buffer() {
		delete buf;
	}
};

#endif /* PIPELINEDATA_H_ */
