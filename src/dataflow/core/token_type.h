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

#ifndef DF_TOKENTYPE_H
#define DF_TOKENTYPE_H

#include <iostream>
#include <string>
#include <cstring>

namespace df {
 
  template <typename T>	
  class TokenType  {
  public:
    T * data;
    TokenType() {
	data = new T();
    }
    virtual int size() = 0;
    virtual void set(const T& d) { *data = d; }
    virtual T get() { return *data; }
    virtual std::string to_string() = 0;
    virtual void from_bytes(char * buf) = 0;   
    virtual char * to_bytes() = 0;   
    virtual ~TokenType() { delete data; }
  };
  
}
#endif // DF_TOKENTYPE_H
