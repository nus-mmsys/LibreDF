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
  private:
	char * chdata;
  public:
  
    Str():Token<std::string>() {
   	chdata = new char[1024+sizeof(int)];
    }
    virtual std::string to_string() { return *data; }

    virtual char * serialize() { 
	if (size == 0) {
		size = 1024;
		memcpy(chdata, &size, sizeof(int));
	}	
	std::strcpy(chdata+sizeof(int), data->c_str());
	return chdata;
    }

    virtual void deserialize(char * buf) {
	*data = buf+sizeof(int);
    }
    virtual ~Str() {
   	delete chdata;
    }
  };

};

#endif /* DF_STR_H_ */
