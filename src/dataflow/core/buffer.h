/*
 *   TMF
 *   Copyright (C) TMF Team
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DF_BUFFER_H_
#define DF_BUFFER_H_

namespace df {
  
  class BufferInfc {
  public:
    virtual void addConsumer() = 0;
  };

  /*!
   * \class Buffer
   *
   * Buffer is a circular list of data.
   * Buffer is used in output ports.
   */
  
  template <typename T>
  class Buffer : public BufferInfc {
  protected:
    T ** tokens;  /**< The array containing the tokens */
    int size;  /**< The size of the buffer */
    
  public:
    
    /*!
     * Buffer constructor
     *
     * \param size the size of the buffer
     */
    Buffer<T>(int s): size(s) {
      
      tokens = new T*[s];

      for (int i=0; i<s; i++) {
	tokens[i] = new T();
      }
    }
    
    void addConsumer()  {
      for (int i=0; i<size; i++) {
	tokens[i]->addConsumer();
      }
    }
     
    void clearConsumer()  {
      for (int i=0; i<size; i++) {
	      if (tokens[i] != nullptr) 
		     tokens[i]->clearConsumer();
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
    T * at(int idx) const  { return tokens[idx]; }
    
    /*!
     * Buffer destructor
     *
     */
    ~Buffer<T>() {
      for (int i=0; i<size; i++) {
	delete tokens[i];
	tokens[i] = nullptr;
	size = 0;
      }
    }
    
  };
  
}

#endif /* DF_BUFFER_H_ */
