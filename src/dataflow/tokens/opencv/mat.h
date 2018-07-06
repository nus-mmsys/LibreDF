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

#include "core/token.h"
#include "opencv2/core/core.hpp"

namespace df {

  class Mat : public Token<cv::Mat> {
  private:
    int rows;
    int cols;
    int type;
    char * chdata;
    int cpysize;
  public:
  
    Mat():Token<cv::Mat>() {
    	rows = 0;
    	cols = 0;
    	type = 0;
    	chdata = nullptr;
    }
    int imgsize() {
	//cv::Size size = data->size();
	//return size.width * size.height * data->channels();
    	return data->total()*data->elemSize();
    }
    virtual std::string to_string() { 
	int total = imgsize(); 
	std::vector<uchar> dv(data->ptr(), data->ptr() + total);
        std::string res(dv.begin(), dv.end());
	return res;
    }
    virtual void set(const cv::Mat& d) {
	*data = d.clone();
    }
    virtual cv::Mat clone() {
	return data->clone();
    }
    
    virtual char * serialize() {
	if (rows == 0 || cols == 0 || size == 0) {
		size = imgsize()+3*sizeof(int);
		cpysize = imgsize();
		if (chdata!=nullptr)
			delete chdata;
		chdata = new char[size+sizeof(int)];
		rows = data->rows;
		cols = data->cols;
		type = data->type();
		memcpy(chdata, &size, sizeof(int)); 
		memcpy(chdata+sizeof(int), &rows, sizeof(int)); 
		memcpy(chdata+2*sizeof(int), &cols, sizeof(int)); 
		memcpy(chdata+3*sizeof(int), &type, sizeof(int));
	}
	memcpy(chdata+4*sizeof(int), reinterpret_cast<char*>(data->data), cpysize);	
	return chdata;
    }

    virtual void deserialize(char * buf) {
	if (rows == 0 || cols == 0 || size == 0) {
		size = pktsize(buf);
		cpysize = size - 3*sizeof(int);
		memcpy(&rows, buf+sizeof(int), sizeof(int)); 
		memcpy(&cols, buf+2*sizeof(int), sizeof(int)); 
		memcpy(&type, buf+3*sizeof(int), sizeof(int)); 
		data = new cv::Mat(rows, cols, type);
	}
	memcpy(data->data, reinterpret_cast<uchar*>(buf+4*sizeof(int)), cpysize);
    }

    virtual ~Mat() { 
    	delete chdata;
    }
  };

};

#endif /* DF_MAT_H_ */
