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

#include "image_write_unite.h"

using namespace df;
using namespace std;

ActorRegister<ImageWriteUnite> ImageWriteUnite::reg("ImageWriteUnite");

ImageWriteUnite::ImageWriteUnite(const string& name) : Actor(name){
  input = createInputPort<df::Mat>("input");
}

void ImageWriteUnite::init() {

}

void ImageWriteUnite::run() {

  df::Mat * in = consume(input);	
  frame = in->clone();
  log("writing image "+to_string(stepno));
  release(input);

  file_name = dfout_path + std::to_string(stepno) + ".png";
  cv::imwrite(file_name, frame); 
  
}

ImageWriteUnite::~ImageWriteUnite() {
  destroyPort(input);
}
