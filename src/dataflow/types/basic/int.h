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

#include "core/token_type.h"
#include <cstdio>

namespace df {

  class Int : public TokenType<int> {

  public:
  
    Int():TokenType<int>() { }
    std::string to_string() { return std::to_string(*data); }
    virtual void from_bytes(char * buf) {
	try {
		*data = atoi(buf);
	} catch (...) {
		std::cerr << "int: from_bytes failed.\n";
	}
    }
    virtual char * to_bytes() { 
	try {
	    sprintf(chdata, "%d", *data);
        } catch(...) {
    		std::cerr << "int: from_bytes failed.\n";
	}
	return chdata;
    }
  
    virtual ~Int() { }
  };

};

#endif /* DF_INT_H_ */
