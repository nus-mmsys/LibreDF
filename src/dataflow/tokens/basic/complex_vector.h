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

#ifndef DF_COMPLEXVECTOR_H_
#define DF_COMPLEXVECTOR_H_

#include "core/token.h"
#include <cstdio>
#include <complex>
#include <vector>
#include <cmath>

namespace df {

  class ComplexVector : public Token<std::vector<std::complex<double>>> {
  public:
  
    ComplexVector():Token<std::vector<std::complex<double>>>() {
    }

    virtual std::string to_string() { 
	    std::string res = "";
	    for (auto c : *data) {
		    res += std::to_string(c.real())+"+"
			    +std::to_string(c.imag())+"i,";
	    }
    	    return res;
    }

    int vector_size() {
	    return data->size();
    }

    std::complex<double> get_item(int i) {
	    return (*data)[i];
    }

    void put_item(std::complex<double> item) {
	    data->push_back(item);
    }

    void clear_items() {
	    data->clear();
    }

    void swap_item(int ii, int jj) {
	    std::swap((*data)[ii], (*data)[jj]);
    }

    virtual int data_size() {
	    return data->size()*sizeof(std::complex<double>)+sizeof(int);
    }

    virtual void serialize_data(char * buf) { 
	    int s = data->size();
	    std::memcpy(buf, &s, sizeof(int));
	    for (auto c : *data) {
	    	std::memcpy(buf, &c, sizeof(std::complex<double>));
	    }
    }

    virtual void deserialize_pkt(char * buf) {
	    int s;
	    std::complex<double> c;
	    std::memcpy(&s, buf, sizeof(int));
	    for (int i=0; i<s; i++) {
	    	std::memcpy(&c, buf, sizeof(std::complex<double>));
		data->push_back(c);
	    }
    }
      
    virtual ~ComplexVector() { }
  };

};

#endif /* DF_COMPLEXVECTOR_H_ */
