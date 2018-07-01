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

#include "core/token_type.h"

namespace df {

  class Str : public TokenType {

  private:
    std::string data; 
  public:
  
    Str() { }
    void set(std::string d) { data = d; } 
    std::string get() { return data; } 
    virtual std::string to_string() { return data; }
    virtual void from_bytes(char * buf) {
	data = buf;
    }
    virtual char * to_bytes() { return nullptr; }
  
    virtual ~Str() { }
  };

};

#endif /* DF_STR_H_ */
