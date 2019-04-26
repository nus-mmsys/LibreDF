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

#include "image_write_merge.h"

using namespace df;
using namespace std;

ActorRegister<ImageWriteMerge> ImageWriteMerge::reg("ImageWriteMerge");

ImageWriteMerge::ImageWriteMerge(const string& name) : Actor(name){
  input = createInputPortVector<df::Mat>("input");
}

void ImageWriteMerge::init() {

  if (propEmpty("level"))
	  level = 1;
  else
	  level = getPropInt("level");

  tilew = 0;
  tileh = 0;

  input->setArity(level * level);

}

void ImageWriteMerge::run() {

  auto in = consume(input);	

  if (tilew == 0 || tileh == 0) { 
  	tilew = in[0]->get()->cols;
  	tileh = in[0]->get()->rows;
	type = in[0]->get()->type();
	frame = cv::Mat(tileh*level, tilew*level, type);
  }

  for (int j=0; j < level ; j++) {
  	for (int i=0; i < level ; i++) {
		cv::Rect tile(i * tilew,
				j * tileh,
				tilew, tileh);

		in[j*level+i]->get()->copyTo(frame(tile));
	}
  }

  log("writing image "+to_string(stepno));
  release(input);
  
  file_name = fsys.outPath() + std::to_string(stepno) + ".png";
  cv::imwrite(file_name, frame); 
}

ImageWriteMerge::~ImageWriteMerge() {
  destroyPort(input);
}
