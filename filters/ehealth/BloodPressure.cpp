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

#include "filters/ehealth/BloodPressure.h"
#include "filters/ehealth/tools/eHealth.h"

using namespace tmf;
using namespace std;

FilterRegister<BloodPressureSensor> BloodPressureSensor::reg("bloodpressure");

BloodPressureSensor::BloodPressureSensor(const string & name) :
Filter(name) {
  outputBloodPressure = createOutputPort<BloodPressureData>("blood pressure output");
}

void BloodPressureSensor::init() {
  eHealth.readBloodPressureSensor();
  delay(100);
  this->period = stoi(getProp("period"));
}

void BloodPressureSensor::run() {
  
  //std::this_thread::sleep_for(std::chrono::seconds(period));
  delay(20000);
  
  uint8_t numberOfData = eHealth.getBloodPressureLength();   
  
  delay(100);

  for (int i = 0; i<numberOfData; i++) {
    
    outputBloodPressure->lock();    
    BloodPressureData * outputBloodPressureData =  outputBloodPressure->get();
    (*outputBloodPressureData).t.day = eHealth.bloodPressureDataVector[i].day;
    (*outputBloodPressureData).t.month = eHealth.bloodPressureDataVector[i].month;
    (*outputBloodPressureData).t.year = eHealth.bloodPressureDataVector[i].year;
    (*outputBloodPressureData).t.hour = eHealth.bloodPressureDataVector[i].hour;
    (*outputBloodPressureData).t.minute = eHealth.bloodPressureDataVector[i].minutes;
    (*outputBloodPressureData).diastolic = eHealth.bloodPressureDataVector[i].diastolic;
    (*outputBloodPressureData).systolic = eHealth.bloodPressureDataVector[i].systolic;
    (*outputBloodPressureData).pulse = eHealth.bloodPressureDataVector[i].pulse;
    outputBloodPressure->unlock(); 

  }
  
}

BloodPressureSensor::~BloodPressureSensor() {
  destroyPort(outputBloodPressure);
}
