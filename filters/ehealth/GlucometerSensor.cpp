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

#include "filters/ehealth/GlucometerSensor.h"
#include "filters/ehealth/tools/eHealth.h"

using namespace tmf;
using namespace std;

FilterRegister<GlucometerSensor> GlucometerSensor::reg("glucometer");

GlucometerSensor::GlucometerSensor(const string & name) :
Filter(name) {
  outputGlucometer = createOutputPort<GlucometerData>("glucometer output");
}

void GlucometerSensor::init() {
  eHealth.readGlucometer();
  delay(100);
  this->period = stoi(getProp("period"));
}

void GlucometerSensor::run() {
  
  //std::this_thread::sleep_for(std::chrono::seconds(period));
  delay(20000);
  
  uint8_t numberOfData = eHealth.getGlucometerLength();   
  
  delay(100);

  for (int i = 0; i<numberOfData; i++) {
    
    outputGlucometer->lock();    
    GlucometerData * outputGlucometerData =  outputGlucometer->get();
    (*outputGlucometerData).t.day = eHealth.glucoseDataVector[i].day;
    (*outputGlucometerData).t.month = eHealth.glucoseDataVector[i].month;
    (*outputGlucometerData).t.year = eHealth.glucoseDataVector[i].year;
    (*outputGlucometerData).t.hour = eHealth.glucoseDataVector[i].hour;
    (*outputGlucometerData).t.minute = eHealth.glucoseDataVector[i].minutes;
    //(*outputGlucometerData).t.meridian = eHealth.glucoseDataVector[i].meridian;
    (*outputGlucometerData).glucose = eHealth.glucoseDataVector[i].glucose;
    outputGlucometer->unlock(); 
  }
}

GlucometerSensor::~GlucometerSensor() {
  destroyPort(outputGlucometer);
}
