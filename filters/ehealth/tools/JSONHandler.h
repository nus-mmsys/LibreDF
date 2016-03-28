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


#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <ehealthsensor/Sensor.h>
#include <vector>
#include <string>
#include <fstream>

struct JSONData {
  int type;
  std::string macid;
  std::string userid;
  long long timestamp;
  std::vector<std::map<std::string, std::string>> sensorData[10];
};

class JSONHandler
{
private:
  JSONData data;
public:
  void setType(int type);
  void setMacID();
  void setUserID(std::string userid);
  
  long long insertData(ehealthsensor::SensorData * sensor);
  std::string toJSON(); 
};

#endif // JSONHANDLER_H
