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
    int matsize;
  public:
  
    Mat():Token<cv::Mat>(1024) {
    	rows = 0;
    	cols = 0;
    	type = 0;
    }
    int calcMatSize() {
	//cv::Size size = data->size();
	//return size.width * size.height * data->channels();
    	matsize = data->total()*data->elemSize();
    	return matsize;
    }
    virtual std::string to_string() { 
	int total = calcMatSize(); 
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
	if (rows == 0 || cols == 0) {
		size = calcMatSize()+3*sizeof(int);
		matsize = calcMatSize();
		delete pktdata;
		pktdata = new char[size+sizeof(int)];
		rows = data->rows;
		cols = data->cols;
		type = data->type();
		memcpy(pktdata, &size, sizeof(int)); 
		memcpy(pktdata+sizeof(int), &rows, sizeof(int)); 
		memcpy(pktdata+2*sizeof(int), &cols, sizeof(int)); 
		memcpy(pktdata+3*sizeof(int), &type, sizeof(int));
	}
	memcpy(pktdata+4*sizeof(int), reinterpret_cast<char*>(data->data), matsize);	
	return pktdata;
    }

    virtual void deserialize(char * buf) {
	if (rows == 0 || cols == 0) {
		size = pktsize(buf);
		matsize = size - 3*sizeof(int);
		memcpy(&rows, buf+sizeof(int), sizeof(int)); 
		memcpy(&cols, buf+2*sizeof(int), sizeof(int)); 
		memcpy(&type, buf+3*sizeof(int), sizeof(int)); 
		data = new cv::Mat(rows, cols, type);
	}
	memcpy(data->data, reinterpret_cast<uchar*>(buf+4*sizeof(int)), matsize);
    }

    virtual ~Mat() { 
    }
  };

};

#endif /* DF_MAT_H_ */
