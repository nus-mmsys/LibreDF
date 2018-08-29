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

#include "property.h"

using namespace df;

std::string Property::getProp(const std::string & key) {
  auto k = props.find(key);
  
  if (k == props.end())
    return "";
  
  return props[key];
}

std::string Property::getKey(const std::string & val) {
  for (auto p : props)
	  if (p.second == val)
		  return p.first;
  return "";
}

bool Property::propEmpty(const std::string & key) {
  auto k = props.find(key);
  
  if (k == props.end())
    return true;
  
  return false;
}

int Property::getPropInt(const std::string & key) {
  std::string strval = getProp(key);
  int val = -1;
  try {
	val = std::stoi(strval);
  } catch( const std::exception& e ) {
        std::cerr << "error: " << e.what() << '\n' ;
  }
  return val;
}

bool Property::getPropBool(const std::string & key) {
  std::string strval = getProp(key);
  bool val = false;
  if (strval == "true" || strval == "1")
	  val = true;
  if (strval == "false" || strval == "0")
	  val = false;
  return val;
}

float Property::getPropFloat(const std::string & key) {
  std::string strval = getProp(key);
  float val = -1;
  try {
	val = std::stof(strval);
  } catch( const std::exception& e ) {
        std::cerr << "error: " << e.what() << '\n' ;
  }
  return val;
}

void Property::setProp(const std::string & key, const std::string& val) {
  props.insert(std::make_pair(key, val));
}

void Property::replaceProp(const std::string & key, const std::string& val) {
  props.erase(key);
  setProp(key, val);
}

void Property::setProp(const std::string & key, const char* val) {
  std::string valstr = std::string(val);
  props.insert(std::make_pair(key, valstr));
}
