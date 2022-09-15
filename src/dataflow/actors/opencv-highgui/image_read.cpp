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

#include "image_read.h"

using namespace df;
using namespace std;

ActorRegister<ImageRead> ImageRead::reg("ImageRead");

ImageRead::ImageRead(const string& name) : Actor(name){
  output = createOutputPort<df::Mat>("output");
}

void ImageRead::init() {
  if (!propEmpty("dataset"))
    dataset = fsys.inPath() + "/" + getProp("dataset");
  else
    dataset = fsys.inPath() + "/ImageNet/";
  
  DIR* dirp = opendir(dataset.c_str());
  struct dirent * dp;
  std::string extention;
  while ((dp = readdir(dirp)) != NULL) {
    file_name = dp->d_name;
    extention = file_name.substr(file_name.find_last_of(".") + 1);
    if(extention == "png" || extention == "jpg") 
      files.push_back(file_name);
  }
  closedir(dirp);  
}

void ImageRead::run() {

  file_name = files.back();	
  //log("name: "+ dataset+file_name);
  frame = cv::imread(dataset+file_name, cv::IMREAD_COLOR); 
  auto out = produce(output);	
  out->set(frame);
  log("reading image "+to_string(stepno));

  files.pop_back(); 
  if (files.empty())
    setEos(output);

  release(output);
}

ImageRead::~ImageRead() {
  destroyPort(output);
}
