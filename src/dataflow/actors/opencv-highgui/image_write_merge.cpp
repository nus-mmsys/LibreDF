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

#include "image_write_merge.h"

using namespace df;
using namespace std;

ActorRegister<ImageWriteMerge> ImageWriteMerge::reg("ImageWriteMerge");

ImageWriteMerge::ImageWriteMerge(const string& name) : Actor(name){
  input = createInputPortVector<df::Mat>("input");
}

void ImageWriteMerge::init() {

}

void ImageWriteMerge::run() {

  /*
  df::Mat * in = consume(input);	
  frame = in->clone();
  log("writing image "+to_string(stepno));
  release(input);

  file_name = dfout_path + std::to_string(stepno) + ".png";
  cv::imwrite(file_name, frame); 
  */
}

ImageWriteMerge::~ImageWriteMerge() {
  destroyPort(input);
}
