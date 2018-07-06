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

#ifndef DF_INT_H_
#define DF_INT_H_

#include "core/token.h"
#include <cstdio>

namespace df {

  class Int : public Token<int> {
  public:
  
    Int():Token<int>(sizeof(int)) {
    }

    virtual std::string to_string() { return std::to_string(*data); }

    virtual void serialize_data() { 
	    std::memcpy(pktdata+PKTHEAD, data, sizeof(int));
    }

    virtual void deserialize_pkt(char * buf) {
	    std::memcpy(data, buf+PKTHEAD, sizeof(int));
    }
      
    virtual ~Int() { }
  };

};

#endif /* DF_INT_H_ */
