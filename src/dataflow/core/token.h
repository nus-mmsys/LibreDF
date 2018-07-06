/*
 *   libdataflow
 *   Copyright (C) 2018
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

#ifndef DF_TOKEN_H
#define DF_TOKEN_H

#include "synchronized.h"

#include <string>

namespace df {
 
  /*!
   * \enum Status
   */
  enum Status {
    OK, /**< Processed successfully. */
    ERROR, /**< An error occurred while processing. */
    EOS, /**< End of stream occured. */
  };

  template <typename T>
  class Token : public Synchronized {
    
  protected:
    T * data;
    int number;
    Status status;
    char * pktdata;
    int size;
    
  public:

    Token(int pktsize): number(0), status(OK) { 
	    size = pktsize;
	    data = new T(); 
    	    pktdata = new char[size+sizeof(int)];
	    std::memcpy(pktdata, &size , sizeof(int));
    } 
    
    T * get() { return data; }
    
    void setStatus(Status st) {status = st;}
    Status getStatus() {return status;}

    int pktsize(char * buf) {
	    memcpy(&size, buf, sizeof(int));
	    return size;
    }

    virtual void set(const T& d) { *data = d; }
    virtual T clone() { return *data; }
    virtual std::string to_string() = 0; 
    virtual char * serialize() = 0;
    virtual void deserialize(char * buf) = 0; 

    virtual ~Token() {
      delete data;
      delete pktdata;
      data = nullptr;
      pktdata = nullptr;
    }
  };
  
}
#endif // DF_TOKEN_H
