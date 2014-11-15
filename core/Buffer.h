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

#ifndef BUFFER_H_
#define BUFFER_H_

#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;


template <typename T>
class Node {
 
  T val;
  mutex mux;
  
  condition_variable con_cond;
  unsigned int con_num;
  
  condition_variable pro_cond;
  bool prod;
  
public:
  
  Node(): con_num(0), prod(false) {}
  
  void consumerLock() {
    unique_lock<mutex> locker(mux);
    while(prod)
      pro_cond.wait(locker);
    
    con_num++;
  }
  
    
  void consumerUnlock() {
    lock_guard<mutex> locker(mux);
    con_num--;
    con_cond.notify_one();
  }
  
  void producerLock() {
    unique_lock<mutex> locker(mux);
    while(con_num > 0)
      con_cond.wait(locker);
    
    prod = true;
  }
  
  bool producerRTLock() {
    lock_guard<mutex> locker(mux);
    if(con_num > 0)
      return false;
    
    prod = true;
    return true;
  }
  
  void producerUnlock() {
    lock_guard<mutex> locker(mux);
    prod = false;
    pro_cond.notify_all();
  }

  /*!
   * Write a value into a node
   *
   * \param val the value to be written 
   */
  void write(T val) {
    this->val = val; 
  }

  /*!
   * Get the current node in the buffer
   *
   * \return the current element of the buffer
   */
  T read() const {
    return val;
  }
};


/*!
 * \class Buffer
 *
 * Buffer is a circular list of data.
 * Buffer is used in output ports.
 */

template<typename T>
class Buffer2 {
protected:
  Node<T> * nodes;  /**< The array containing the nodes */
  int size;  /**< The size of the buffer */
  
public:
  
  /*!
   * Buffer constructor
   *
   * \param size the size of the buffer
   */
  Buffer2(int s): size(s), nodes(new Node<T>[s]())  {
  }
  
  /*!
   * Get the size of the buffer
   *
   * \return the size of the buffer
   */ 
  int getSize() const {return size;}

  /*!
   * Get an element of the buffer by index
   *
   * \param idx the number of the element
   * \return the element number idx
   */
  Node<T>& at(int idx) const  { return nodes[idx]; }

  /*!
   * Buffer destructor
   *
   */
  ~Buffer2() {
    delete nodes;
  }
  
};


template<class Type>
class Buffer {

private:

	Type ** buf;
	int curIndex; /**< the current index in the buffer */
	int size;

public:

	
	Buffer(int size) {
		buf = new Type*[size];
		curIndex = -1;
		this->size = size;

		for (int i = 0; i < size; i++)
			buf[i] = new Type();
	}


	void insert(Type * e) {
		curIndex = (curIndex + 1) % size;
		buf[curIndex] = e;
	}


	Type * getNode() {
		return buf[curIndex];
	}


	int getSize() {
		return size;
	}


	Type * getNode(int i) {
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
	Type * getNextNode() {
		int nextIndex = (curIndex + 1) % size;
		return buf[nextIndex];
	}


	~Buffer() {
		for (int i = 0; i < size; i++)
			delete buf[i];

		delete buf;
	}
};

#endif /* BUFFER_H_ */
