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


#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include "../ehealthsensor/include/Sensor.h"
#include <vector>
#include <string>

using namespace ehealthsensor;

struct JSONData {
  std::string userid;
  long long timestamp[10];
  std::vector<std::map<std::string, std::string>> sensorData[10];
};

class JSONHandler
{
public:
  JSONData data;
  void insertData(SensorData * sensor);
  std::string toJSON(); 
};

#endif // JSONHANDLER_H
