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

#ifndef DF_MATMERGE_H_
#define DF_MATMERGE_H_

#include "core/df.h"
#include "tokens/opencv/mat.h"

#include <opencv2/core/core.hpp>

#include <iostream>
#include <string>

using namespace std;

class MatMerge: public df::Actor {
  
private:

  cv::Mat frame;
  df::InputPort<df::Mat> * input;
  df::OutputPort<df::Mat> * output;
 
  static  df::ActorRegister<MatMerge> reg;
public:
  
  MatMerge(const string& name);
  
  virtual void init();
  
  virtual void run();
  
  virtual ~MatMerge();
  
};

#endif /* DF_MATMERGE_H_ */
