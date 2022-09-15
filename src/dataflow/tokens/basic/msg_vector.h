/*
 *   LibreDF
 *   Copyright (C) A. Shafiei
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

#ifndef DF_MSGVECTOR_H_
#define DF_MSGVECTOR_H_

#include "core/token.h"
#include <cstdio>
#include <vector>
#include <cmath>

namespace df {

  struct MsgStruct {
	  unsigned long long header;
	  std::string message;
  };

  class MsgVector : public Token<std::vector<MsgStruct>> {
  public:
  
    MsgVector():Token<std::vector<MsgStruct>>() {
    }

    virtual std::string to_string() { 
	    std::string res = "";
	    for (auto c : *data) {
		    res += std::to_string(c.header) + 
			    ":" + c.message + " ";
	    }
    	    return res;
    }

    int vector_size() {
	    return data->size();
    }

    unsigned long long get_item_header(int i) {
	    return (*data)[i].header;
    }

    std::string get_item_message(int i) {
	    return (*data)[i].message;
    }

    void put_item(unsigned long long h, std::string m) {
	    MsgStruct msg;
	    msg.header = h;
	    msg.message = m;
	    data->push_back(msg);
    }

    void clear_items() {
	    data->clear();
    }

    void swap_item(int ii, int jj) {
	    std::swap((*data)[ii], (*data)[jj]);
    }

    virtual int data_size() {
	    return 1024+sizeof(unsigned long long);
    }

    virtual void serialize_data(char * buf) { 
	    int s = data->size();
	    std::memcpy(buf, &s, sizeof(int));
	    for (auto c : *data) {
	    	std::memcpy(buf, &c.header, sizeof(unsigned long long));
		std::memcpy(buf, &c.message, 1024);
	    }
    }

    virtual void deserialize_pkt(char * buf) {
	    int s;
	    MsgStruct c;
	    std::memcpy(&s, buf, sizeof(int));
	    for (int i=0; i<s; i++) {
	    	std::memcpy(&c.header, buf, sizeof(unsigned long long));
	    	std::memcpy(&c.message, buf, 1024);
		data->push_back(c);
	    }
    }
      
    virtual ~MsgVector() { }
  };

};

#endif /* DF_MSGVECTOR_H_ */
