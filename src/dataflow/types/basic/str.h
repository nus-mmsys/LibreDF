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

  class Str : public TokenType<std::string> {
  private:
	char * chdata;
  public:
  
    Str():TokenType<std::string>() {
   	chdata = new char[1024];
    }
    virtual int size() {
	    return 1024;
    }
    virtual std::string to_string() { return *data; }
    virtual void from_bytes(char * buf) {
	*data = buf;
    }
    virtual char * to_bytes() { 
	std::strcpy(chdata, data->c_str());
	return chdata;
    }
    virtual ~Str() {
   	delete chdata;
    }
  };

};

#endif /* DF_STR_H_ */
