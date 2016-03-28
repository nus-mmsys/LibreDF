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
  scheduler = new Scheduler(50,20000);
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
  if (scheduler->timeToExecute(sensor->getSamplingPeriod()))
  {
    output->lock();
    SensorData * outputData = output->get();
    sensor->read(outputData);
    output->unlock();
  }   
}

void EHealthSensor::run() {
  
  scheduler->start();
  
  //readSensor(&airflowSensor);
  //readSensor(&bloodpressureSensor);
  //readSensor(&ecgSensor);
  //readSensor(&emgSensor);
  //readSensor(&galvanicSensor);
  //readSensor(&glucometerSensor);
  //readSensor(&positionSensor);
  readSensor(&pulsioximeterSensor);
  readSensor(&temperatureSensor);
  
  scheduler->end();
  

}

EHealthSensor::~EHealthSensor() {
  destroyPort(output);
}
