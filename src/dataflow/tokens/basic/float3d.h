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

#ifndef DF_FLOAT3D_H_
#define DF_FLOAT3D_H_

#include "core/token.h"
#include <cstdio>

namespace df {

  struct Float3DStruct {
	float x;
	float y;
	float z;
  };

  class Float3D : public Token<Float3DStruct> {
  public:
  
    Float3D():Token<Float3DStruct>() {
    }

    virtual std::string to_string() { 
	    //TODO
	    return "Float3D"; 
    }

    virtual int data_size() {
	    return sizeof(Float3DStruct);
    }

    virtual void serialize_data(char * buf) { 
	    //TODO
	    std::memcpy(buf, data, sizeof(Float3DStruct));
    }

    virtual void deserialize_pkt(char * buf) {
	    //TODO
	    std::memcpy(data, buf, sizeof(Float3DStruct));
    }
      
    virtual ~Float3D() { }
  };

};

#endif /* DF_FLOAT3D_H_ */
