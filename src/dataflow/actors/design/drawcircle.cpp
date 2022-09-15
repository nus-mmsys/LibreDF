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

#include "drawcircle.h"

using namespace df;
using namespace std;

ActorRegister<DrawCircle> DrawCircle::reg("DrawCircle");

DrawCircle::DrawCircle(const string& name) : Actor(name) {
  input = createInputPort<df::Float3D>("input");
  output = createOutputPort<df::Mat>("output");

}

void DrawCircle::init() {
  if (!propEmpty("scale"))
	 scale = getPropInt("scale");
  else 
	 scale = 200; 
  img = cv::Mat(600, 800, CV_8UC3, cv::Scalar(255,255,255));  
  center = cv::Point(400,300);
}

void DrawCircle::run() {

  Float3D * in = consume(input);
  Mat * out = produce(output);
  auto c = in->get();
  cv::circle(img, center+cv::Point(c->x*scale, c->y*-1*scale),
       c->z*scale, cv::Scalar(0,0,0));
  out->set(img);
  log("circle "+to_string(c->x)+","+to_string(c->y)) ;
  
  release(output);  
  release(input);
}


DrawCircle::~DrawCircle() {
  destroyPort(output);
}
