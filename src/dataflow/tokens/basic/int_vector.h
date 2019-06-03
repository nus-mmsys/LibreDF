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

#ifndef DF_INTVECTOR_H_
#define DF_INTVECTOR_H_

#include "core/token.h"
#include <cstdio>
#include <vector>
#include <cmath>

namespace df {

  class IntVector : public Token<std::vector<int>> {
  public:
  
    IntVector():Token<std::vector<int>>() {
    }

    virtual std::string to_string() { 
	    std::string res = "";
	    for (auto c : *data) {
		    res += std::to_string(c);
	    }
    	    return res;
    }

    int vector_size() {
	    return data->size();
    }

    int get_item(int i) {
	    return (*data)[i];
    }

    void put_item(int item) {
	    data->push_back(item);
    }

    void clear_items() {
	    data->clear();
    }

    void swap_item(int ii, int jj) {
	    std::swap((*data)[ii], (*data)[jj]);
    }

    virtual int data_size() {
	    return (data->size()+1)*sizeof(int);
    }

    virtual void serialize_data(char * buf) { 
	    int s = data->size();
	    std::memcpy(buf, &s, sizeof(int));
	    for (auto c : *data) {
	    	std::memcpy(buf, &c, sizeof(int));
	    }
    }

    virtual void deserialize_pkt(char * buf) {
	    int s;
	    int c;
	    std::memcpy(&s, buf, sizeof(int));
	    for (int i=0; i<s; i++) {
	    	std::memcpy(&c, buf, sizeof(int));
		data->push_back(c);
	    }
    }
      
    virtual ~IntVector() { }
  };

};

#endif /* DF_INTVECTOR_H_ */
