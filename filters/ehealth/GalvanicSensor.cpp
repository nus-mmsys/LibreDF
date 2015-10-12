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

#include "filters/ehealth/GalvanicSensor.h"
#include "filters/ehealth/tools/eHealth.h"

using namespace tmf;
using namespace std;

FilterRegister<GalvanicSensor> GalvanicSensor::reg("galvanic");

GalvanicSensor::GalvanicSensor(const string & name) :
Filter(name) {
  outputGalvanic = createOutputPort<GalvanicData>("galvanic output");
}

void GalvanicSensor::init() {
  this->period = stoi(getProp("period"));
}

void GalvanicSensor::run() {
  
  std::this_thread::sleep_for(std::chrono::seconds(period));
  
  outputGalvanic->lock();
  
  GalvanicData * outputGalvanicData =  outputGalvanic->get();
  (*outputGalvanicData).conductance = eHealth.getSkinConductance();
  (*outputGalvanicData).resistance = eHealth.getSkinResistance();
  (*outputGalvanicData).voltage = eHealth.getSkinConductanceVoltage();
  (*outputGalvanicData).t.setCurrentTime();
  outputGalvanic->unlock();
  
}

GalvanicSensor::~GalvanicSensor() {
  destroyPort(outputGalvanic);
}