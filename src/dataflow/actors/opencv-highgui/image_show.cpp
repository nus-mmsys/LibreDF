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

#include "image_show.h"

using namespace df;
using namespace std;

ActorRegister<ImageShow> ImageShow::reg("ImageShow");

ImageShow::ImageShow(const string& name) : Actor(name) {
  inputMat = createInputPort<cv::Mat>("opencv input");
}

void ImageShow::init() {
 

}

void ImageShow::run() {

  cv::Mat * in = consume(inputMat);	
  frame = in->clone();
  log("capturing frame "+to_string(stepno));
  release(inputMat);
 
  cv::imshow("ImageShow", frame);
  cv::waitKey(1000);

}

ImageShow::~ImageShow() {
  destroyPort(inputMat);
  cv::destroyAllWindows();
}
