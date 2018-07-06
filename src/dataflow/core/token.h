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

  #define PKTHEAD 2*sizeof(int)
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
    char * pkt;
    int pktsize;
    
  public:

    Token(int size): number(0), status(OK) { 
	    data = new T(); 
	    pkt = nullptr;
	    initPacket(size);
    } 
   
    void initPacket(int size) {
	    if (pkt != nullptr)
		    delete pkt;
	    pktsize = size+PKTHEAD;
	    pkt = new char[pktsize];
	    std::memcpy(pkt, &pktsize , sizeof(int));
	    std::memcpy(pkt+sizeof(int), &status , sizeof(int));
    }

    T * get() { return data; }
    
    void setStatus(Status st) {status = st;}
    Status getStatus() {return status;}

    int getPktSize(char * buf) {
	    memcpy(&pktsize, buf, sizeof(int));
	    return pktsize;
    }

    int getDataSize() {
	    return pktsize - PKTHEAD;
    }

    char * serialize() {
    	std::memcpy(pkt+sizeof(int), &status, sizeof(int));
    	serialize_data();
	return pkt;	
    }

    void deserialize(char * buf) {
	std::memcpy(&status, buf+sizeof(int), sizeof(int));
    	deserialize_pkt(buf);
    }

    virtual void set(const T& d) { *data = d; }
    virtual T clone() { return *data; }
    virtual std::string to_string() = 0; 
    virtual void serialize_data() = 0;
    virtual void deserialize_pkt(char *) = 0; 

    virtual ~Token() {
      delete data;
      delete pkt;
      data = nullptr;
      pkt = nullptr;
    }
  };
  
}
#endif // DF_TOKEN_H
