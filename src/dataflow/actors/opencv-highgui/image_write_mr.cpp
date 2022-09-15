/*
 *   LibreDF
 *   Copyright (C) A. Shafiei
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

#include "image_write_mr.h"

using namespace df;
using namespace std;

ActorRegister<ImageWriteMR> ImageWriteMR::reg("ImageWriteMR");

ImageWriteMR::ImageWriteMR(const string& name) : Actor(name){
  input = createInputPort<df::Mat>("input");
}

void ImageWriteMR::init() {

}

void ImageWriteMR::run() {

  auto in = consumeMR(input);	

  for (int j=0; j<in.size(); j++) {
  	file_name = fsys.outPath() + 
		std::to_string(stepno) + "-" + 
		std::to_string(j) + ".png";
	cv::imwrite(file_name, *in[j]->get()); 
  }
 
  log("writing image "+to_string(stepno));
  releaseMR(input);
}

ImageWriteMR::~ImageWriteMR() {
  destroyPort(input);
}
