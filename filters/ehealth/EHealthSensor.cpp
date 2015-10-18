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

#include "filters/ehealth/EHealthSensor.h"

#include <chrono>

using namespace tmf;
using namespace std;
using namespace ehealthsensor;
using namespace std::chrono;

FilterRegister<EHealthSensor> EHealthSensor::reg("ehealthsensor");

EHealthSensor::EHealthSensor(const string & name) :
Filter(name) {
  output = createOutputPort<SensorData>("ehealthdata output");
  currentTime = 0;
  minPeriod = 50;
  maxPeriod = 20000;
}

void EHealthSensor::init() {
  airflowSensor.setup();
  bloodpressureSensor.setup();
  ecgSensor.setup();
  emgSensor.setup();
  galvanicSensor.setup();
  glucometerSensor.setup();
  positionSensor.setup();
  pulsioximeterSensor.setup();  
  temperatureSensor.setup();
}

void EHealthSensor::readSensor(Sensor * sensor) {
  if (currentTime % sensor->getSamplingPeriod() == 0)
  {
    output->lock();
    SensorData * outputData = output->get();
    sensor->read(outputData);
    output->unlock();
  }   
}

void EHealthSensor::run() {
  high_resolution_clock::time_point start = high_resolution_clock::now();
  
//  readSensor(&airflowSensor);
//  readSensor(&temperatureSensor);
  readSensor(&pulsioximeterSensor);
  
  high_resolution_clock::time_point end = high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count();

  //cout << "Duration:" << duration << endl;
  
  std::this_thread::sleep_for(std::chrono::milliseconds(minPeriod - duration/1000));

  currentTime+=minPeriod;
  if (currentTime >=  maxPeriod)
    currentTime = 0;
}

EHealthSensor::~EHealthSensor() {
  destroyPort(output);
}
