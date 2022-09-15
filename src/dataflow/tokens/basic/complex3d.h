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

#ifndef DF_COMPLEX3D_H_
#define DF_COMPLEX3D_H_

#include "core/token.h"
#include <cstdio>
#include <complex>
#include <cmath>

namespace df {

  struct Complex3DStruct {
	std::complex<double> x;
	std::complex<double> y;
	std::complex<double> z;
  };

  class Complex3D : public Token<Complex3DStruct> {
  public:
  
    Complex3D():Token<Complex3DStruct>() {
    }

    virtual std::string to_string() { 
	    //TODO
	    return "Complex3D"; 
    }

    virtual int data_size() {
	    return sizeof(Complex3DStruct);
    }

    virtual void serialize_data(char * buf) { 
	    //TODO
	    std::memcpy(buf, data, sizeof(Complex3DStruct));
    }

    virtual void deserialize_pkt(char * buf) {
	    //TODO
	    std::memcpy(data, buf, sizeof(Complex3DStruct));
    }
      
    virtual ~Complex3D() { }
  };

};

#endif /* DF_COMPLEX3D_H_ */
