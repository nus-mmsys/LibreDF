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

#ifndef DF_MAT_H_
#define DF_MAT_H_

#include "core/token_type.h"
#include "opencv2/core/core.hpp"

namespace df {

  class Mat : public TokenType<cv::Mat> {

  public:
  
    Mat():TokenType<cv::Mat>() { }
    virtual std::string to_string() { 
   	//TODO
	return "";
    }
    virtual void set(const cv::Mat& d) {
	*data = d.clone();
    }
    virtual cv::Mat get() {
	return data->clone();
    }
    cv::Mat&& ref() {
	   return std::move(*data);
    } 
    virtual void from_bytes(char * buf) {
	//TODO	
    }
    virtual char * to_bytes() { 
	//TODO
	//return data->data;
	return nullptr;
    }
    virtual ~Mat() { }
  };

};

#endif /* DF_MAT_H_ */
