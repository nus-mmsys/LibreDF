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

#include "cascade_classifier.h"

using namespace df;
using namespace std;

ActorRegister<CascadeClassifier> CascadeClassifier::reg("CascadeClassifier");

CascadeClassifier::CascadeClassifier(const string& name) : Actor(name) {
  inputGray = createInputPort<df::Mat>("input_gray");
  inputImage = createInputPort<df::Mat>("input_image");
}

void CascadeClassifier::init() {

  string cascadename;
  string cascadepath = df_path + "/haarcascades/";
  if (getProp("classifier") != "")  	
  	cascadename = cascadepath + getProp("classifier");
  else
	cascadename = cascadepath + "haarcascade_fullbody.xml";

  if (!classifier.load(cascadename)) {
    cout << "error while loading the cascade classifier\n";
  }
}

void CascadeClassifier::run() {

  df::Mat * ingray = consume(inputGray);	
  df::Mat * inimg = consume(inputImage);	
  frame = inimg->clone(); 
  classifier.detectMultiScale(*ingray->get(), objects, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
    for (cv::Rect r : objects) {
    log("detecting rect in "+to_string(stepno)+" ("+to_string(r.x)+", "+to_string(r.y)+", "+to_string(r.width)+", "+to_string(r.height)+")");
    cv::rectangle(frame, r, cv::Scalar(0, 255, 0));
  }
  file_name = dfout_path + std::to_string(stepno) + ".png";
  cv::imwrite(file_name, frame); 

  release(inputGray);
  release(inputImage);

}

CascadeClassifier::~CascadeClassifier() {
  destroyPort(inputGray);
  destroyPort(inputImage);
}
