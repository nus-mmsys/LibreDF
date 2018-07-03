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
  private:
    int rows;
    int cols;
    int type;
    char * chdata;
  public:
  
    Mat():TokenType<cv::Mat>() {
    	rows = 0;
    	cols = 0;
    	type = 0;
    }
    virtual int size() {
	    return (data->total()*data*elemSize)+1024;
    }
    virtual std::string to_string() { 
	cv::Size size = data->size();
	int total = size.width * size.height * data->channels();
	std::vector<uchar> dv(data->ptr(), data->ptr() + total);
        std::string res(dv.begin(), dv.end());
	return res;
    }
    virtual void set(const cv::Mat& d) {
	*data = d.clone();
    }
    virtual cv::Mat get() {
	return data->clone();
    }
    virtual void from_bytes(char * buf) {
	if (rows == 0 || cols == 0) {
		memcpy(&rows, buf, sizeof(int)); 
		memcpy(&cols, buf+sizeof(int), sizeof(int)); 
		memcpy(&type, buf+2*sizeof(int), sizeof(int)); 
		std::cout << "rows = " << rows << "\n";
		std::cout << "cols = " << cols << "\n";
		std::cout << "type = " << type << "\n";
	}
	delete data;
	data = new cv::Mat(rows, cols, type, buf+3*sizeof(int));
        //data->data = reinterpret_cast<uchar *>(buf+3*sizeof(int));
    }
    virtual char * to_bytes() {
	if (rows == 0 || cols == 0) {
		if (chdata)
			delete chdata;
		chdata = new char[size()];
		rows = data->rows;
		cols = data->cols;
		type = data->type();
		memcpy(chdata, &rows, sizeof(int)); 
		memcpy(chdata+sizeof(int), &cols, sizeof(int)); 
		memcpy(chdata+2*sizeof(int), &type, sizeof(int));
	}
	memcpy(chdata+3*sizeof(int), reinterpret_cast<char*>(data->data), size-3*sizeof(int)); 
	return chdata;
    }
    virtual ~Mat() { 
    	delete chdata;
    }
  };

};

#endif /* DF_MAT_H_ */
