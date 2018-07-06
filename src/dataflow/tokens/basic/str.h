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

#ifndef DF_STR_H_
#define DF_STR_H_

#include "core/token.h"

namespace df {

  class Str : public Token<std::string> {
  public:
  
    Str():Token<std::string>(1024) {
    }
    
    virtual std::string to_string() { return *data; }

    virtual void serialize_data(char * buf) { 
	std::strcpy(buf, data->c_str());
    }

    virtual void deserialize_pkt(char * buf) {
	*data = buf;
    }
    virtual ~Str() {
    }
  };

};

#endif /* DF_STR_H_ */
