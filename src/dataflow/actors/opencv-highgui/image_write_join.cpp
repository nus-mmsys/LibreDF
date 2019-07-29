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

#include "image_write_join.h"

using namespace df;
using namespace std;

ActorRegister<ImageWriteJoin> ImageWriteJoin::reg("ImageWriteJoin");

ImageWriteJoin::ImageWriteJoin(const string& name) : Actor(name){
  input = createInputPortVector<df::Mat>("input");
}

void ImageWriteJoin::init() {
  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");
}

void ImageWriteJoin::reinit() {
  if (propEmpty("arity"))
	  arity = 1;
  else
	  arity = getPropInt("arity");
}

void ImageWriteJoin::run() {

  auto in = consume(input);	

  for (int i=0; i<in.size(); i++) {

      frame = in[i]->clone();
      log("writing image "+to_string(stepno));
      file_name = fsys.outPath() + std::to_string(stepno) + 
	      + "-" + std::to_string(i) + ".png";
      cv::imwrite(file_name, frame); 
  }

  release(input);
  
}

ImageWriteJoin::~ImageWriteJoin() {
  destroyPort(input);
}
