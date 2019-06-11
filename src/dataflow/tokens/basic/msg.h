/*
 *   TMF
 *   Copyright (C) TMF Team
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

#ifndef DF_MSG_H_
#define DF_MSG_H_

#include "core/token.h"

namespace df {

  struct MsgStruct {
	  int header;
	  std::string message;
  };

  class Msg : public Token<MsgStruct> {
  public:
  
    Msg():Token<MsgStruct>() {
    }
   
    void setHeader(int h) {
	   data->header = h;
    }

    void setMessage(std::string m) {
	   data->message = m;
    }

    virtual std::string to_string() { 
	    return std::to_string(data->header)+" "+(data->message); 
    }

    virtual int data_size() {
	    return 1024;
    }

    virtual void serialize_data(char * buf) { 
	std::memcpy(buf, &data->header, sizeof(int));
	std::strcpy(buf+sizeof(int), data->message.c_str());
    }

    virtual void deserialize_pkt(char * buf) {
	std::memcpy(&data->header, buf, sizeof(int));
	data->message = buf+sizeof(int);
    }
    virtual ~Msg() {
    }
  };

};

#endif /* DF_MSG_H_ */
