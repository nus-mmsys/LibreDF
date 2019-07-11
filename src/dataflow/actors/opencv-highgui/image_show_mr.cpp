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

#include "image_show_mr.h"

using namespace df;
using namespace std;

ActorRegister<ImageShowMR> ImageShowMR::reg("ImageShowMR");

ImageShowMR::ImageShowMR(const string& name) : Actor(name){
  input = createInputPort<df::Mat>("input");
}

void ImageShowMR::init() {

  if (!propEmpty("adjust_period"))  	
  	adjust_period = getPropBool("adjust_period");
  else
        adjust_period = false;
 
  if (!propEmpty("period"))  	
  	period = getPropInt("period");
  else
        period = 40;
 
}

void ImageShowMR::run() {

  auto in = consumeMR(input);	

  for (int j=0; j<in.size(); j++) {

      frame = in[j]->clone();
  
      log("showing image "+to_string(stepno));
      
      cv::imshow("ImageShow", frame);
      if (adjust_period)
      	cv::waitKey(period/(in.size()*getSolution()));
      else
	cv::waitKey(period);
  }
 
  releaseMR(input);
}

ImageShowMR::~ImageShowMR() {
  destroyPort(input);
  cv::destroyAllWindows();
}
