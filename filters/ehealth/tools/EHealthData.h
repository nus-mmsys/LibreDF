/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2015  <copyright holder> <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef EHEALTHDATA_H
#define EHEALTHDATA_H

#include <string>

struct TemperatureData {
  float temperature;
};
struct PulsioximeterData {
  int bpm;
  int oxygen;
};
struct ECGData {
  float ecg;
};
struct EMGData {
  int emg;
};
struct AirflowData {
  int airflow;
};
struct PositionData {
  uint8_t position;
};
struct GalvanicData {
  float conductance;
  float resistance;
  float voltage;
};


class EHealthData
{
public:
EHealthData();
EHealthData(const EHealthData& other);
~EHealthData();
static std::string positionToString( uint8_t position);
};

#endif // EHEALTHDATA_H
