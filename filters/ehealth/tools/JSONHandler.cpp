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

long long JSONHandler::insertData(SensorData* sensor)
{
  if (data.timestamp[sensor->sensorID] == 0 )
    data.timestamp[sensor->sensorID] = sensor->getTimestamp();
  
  long long timediff = sensor->getTimestamp() - data.timestamp[sensor->sensorID];
  std::map<std::string, std::string> curmap = sensor->getMap();
  curmap["timediff"] = std::to_string(timediff);
  data.sensorData[sensor->sensorID].push_back(curmap);
  return timediff;
}


string JSONHandler::toJSON() {
  string res = "{ \"userid\": \"" + data.userid + "\", ";
  for (int i = 1; i <= 9 ; i++) {
    res += ("\"" + std::to_string(i) + "\" : { \"timestamp\":");
    res += ("\"" + std::to_string(data.timestamp) + "\" , \"data\": [ ");
    while(!data.sensorData[i].empty()) {
      res += "{ ";
      std::map<std::string, std::string> curmap = data.sensorData[i].back();
      data.sensorData[i].pop_back();
      for (auto& m : curmap)
        res += ("\"" + m.first + "\" : \"" + m.second + "\" ,");
      res.pop_back();
      res += " } ,";
    }
    res.pop_back();
    res += "] } ,";
    data.timestamp[i] = 0;    
  } 
  res.pop_back();
  res += "}";
  return res;
}

