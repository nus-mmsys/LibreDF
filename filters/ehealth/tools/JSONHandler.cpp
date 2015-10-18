/*
 * 
 *  Tiny Multimedia Framework
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "filters/ehealth/tools/JSONHandler.h"

#include<iostream>

using namespace std;

string JSONHandler::toJSON() {
  string res = "{ \"userid\": \"" + userid + "\", ";
  for (int i = 1; i <= 9 ; i++) {
    res += ("\"" + std::to_string(i) + "\" : { \"timestamp\":");
    res += ("\"" + sensordata[i].timestamp + "\" , \"data\": [ ");
    while(!sensordata[i].data.empty()) {
      res += "{ ";
      map<string, string> curmap = sensordata[i].data.back();
      sensordata[i].data.pop_back();
      for (auto& m : curmap)
        res += ("\"" + m.first + "\" : \"" + m.second + "\" ,");
      res.pop_back();
      res += " } ,";
    }
    res.pop_back();
    res += "] } ,";
    sensordata[i].timestamp = "";    
  } 
  res.pop_back();
  res += "}";
  return res;
}

