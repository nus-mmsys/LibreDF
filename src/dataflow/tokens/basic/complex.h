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

#ifndef DF_COMPLEX_H_
#define DF_COMPLEX_H_

#include "core/token.h"
#include <cstdio>
#include <complex>
#include <cmath>

namespace df {

  class Complex : public Token<std::complex<double>> {
  public:
  
    Complex():Token<std::complex<double>>() {
    }

    virtual std::string to_string() { 
	    return std::to_string(data->real())+"+"
			    +std::to_string(data->imag())+"i"; 
    }

    virtual int data_size() {
	    return sizeof(std::complex<double>);
    }

    virtual void serialize_data(char * buf) { 
	    std::memcpy(buf, data, sizeof(std::complex<double>));
    }

    virtual void deserialize_pkt(char * buf) {
	    std::memcpy(data, buf, sizeof(std::complex<double>));
    }
      
    virtual ~Complex() { }
  };

};

#endif /* DF_COMPLEX_H_ */
