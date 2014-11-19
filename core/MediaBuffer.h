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

#ifndef MEDIABUFFER_H_
#define MEDIABUFFER_H_

#include "MediaSample.h"

const int TMF_BUFFER_SIZE = 3;

/*!
 * \class Buffer
 *
 * Buffer is a circular list of data.
 * Buffer is used in output ports.
 */

template <typename T>
class MediaBuffer {
protected:
  MediaSample<T> * samples[TMF_BUFFER_SIZE];  /**< The array containing the samples */
  int size;  /**< The size of the buffer */
  
public:
  
  /*!
   * Buffer constructor
   *
   * \param size the size of the buffer
   */
  MediaBuffer<T>(int s): size(s) {
    
    for (int i=0; i<size; i++) {
      samples[i] = new MediaSample<T>();
    }
  }
 
  void addConsumer() {
    for (int i=0; i<size; i++) {
      samples[i]->addConsumer();
    }
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
  MediaSample<T>* at(int idx) const  { return samples[idx]; }

  /*!
   * Buffer destructor
   *
   */
  ~MediaBuffer<T>() {
    for (int i=0; i<size; i++) {
      delete samples[i];
    }
  }
  
};

#endif /* MEDIABUFFER_H_ */
