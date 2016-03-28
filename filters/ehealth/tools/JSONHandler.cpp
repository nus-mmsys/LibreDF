/*
 * 
 *  Tiny Multimedia Framework (tmf)
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This file is part of tmf.
 *
 *  tmf is free software; you can redistribute it and/or 
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  tmf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with tmf.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "filters/ehealth/tools/JSONHandler.h"

#include<iostream>

using namespace std;
using namespace ehealthsensor;

long long JSONHandler::insertData(SensorData* sensor)
{
  int sid = sensor->getSensorID();
  if (data.timestamp == 0 )
    data.timestamp = sensor->getTimestamp();
  
  long long timediff = sensor->getTimestamp() - data.timestamp;
  std::map<std::string, std::string> curmap = sensor->getMap();
  curmap["timediff"] = std::to_string(timediff);
  data.sensorData[sid].push_back(curmap);
  return timediff;
}

void JSONHandler::setType(int type)
{
  data.type = type;
}

void JSONHandler::setMacID()
{
  std::ifstream ifs("/sys/class/net/eth0/address");
  data.macid.assign((std::istreambuf_iterator<char>(ifs)),
                                (std::istreambuf_iterator<char>()));
  data.macid.erase(data.macid.find_last_not_of(" \n\r\t")+1);
}

void JSONHandler::setUserID(string userid)
{
  data.userid = userid;
}

string JSONHandler::toJSON() {
  string res =  "{ \"type\": \"" + std::to_string(data.type) + "\", ";
  res += "\"mac\": \"" + data.macid + "\", ";
  res += "\"userid\": \"" + data.userid + "\" ,";

  if (data.type == 0) {
    res += "\"timestamp\": \"" + std::to_string(data.timestamp) + "\",";
  
    for (int i = 1; i <= 9 ; i++) {
      res += ("\"" + std::to_string(i) + "\" : [ ");
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
      res += "]  ,";  
    } 
    data.timestamp = 0;  
  }    

  res.pop_back();
  res += "}";
  return res;
}

