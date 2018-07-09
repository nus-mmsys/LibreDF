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

    Mat():Token<cv::Mat>() {
    	rows = 0;
    	cols = 0;
    	type = 0;
	matsize = 0;
    }

    int calcMatSize() {
	//cv::Size size = data->size();
	//return size.width * size.height * data->channels();
    	return data->total()*data->elemSize();
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

    virtual int data_size() {
	return calcMatSize() + 3*sizeof(int);
    }

    virtual void serialize_init(char * buf) {
	rows = data->rows;
	cols = data->cols;
	type = data->type();
	matsize = calcMatSize();
	memcpy(buf, &rows, sizeof(int)); 
	memcpy(buf+sizeof(int), &cols, sizeof(int)); 
	memcpy(buf+2*sizeof(int), &type, sizeof(int));
    }

    virtual void serialize_data(char * buf) {
	memcpy(buf+3*sizeof(int), reinterpret_cast<char*>(data->data), matsize);
    }

    virtual void deserialize_init(char * buf, int dsize) {
	matsize = dsize - 3*sizeof(int);
	memcpy(&rows, buf, sizeof(int)); 
	memcpy(&cols, buf+sizeof(int), sizeof(int)); 
	memcpy(&type, buf+2*sizeof(int), sizeof(int)); 
	data = new cv::Mat(rows, cols, type);
    }

    virtual void deserialize_pkt(char * buf) {
	memcpy(data->data, reinterpret_cast<uchar*>(buf+3*sizeof(int)), matsize);
    }

    virtual ~Mat() { 
    }
  };

};

#endif /* DF_MAT_H_ */
