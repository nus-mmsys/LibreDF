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

#include "filters/ehealth/PulsioximeterSensor.h"
#include "filters/ehealth/tools/eHealth.h"

using namespace tmf;
using namespace std;

FilterRegister<PulsioximeterSensor> PulsioximeterSensor::reg("pulsioximeter");

PulsioximeterSensor::PulsioximeterSensor(const string & name) :
Filter(name) {
  outputPulse = createOutputPort<PulseData>("pulse output");
  outputOxi = createOutputPort<OxiData>("oxi output");
}

void PulsioximeterSensor::init() {
  
  this->period = stoi(getProp("period"));
  
  cont=0;
  eHealth.initPulsioximeter();
  attachInterrupt(6, PulsioximeterSensor::readPulsioximeter, RISING);

}

void PulsioximeterSensor::readPulsioximeter() {
  cont++;
  if (cont==500) {
    eHealth.readPulsioximeter();
    cont = 0;
  }
}

void PulsioximeterSensor::run() {
  
  std::this_thread::sleep_for(std::chrono::seconds(period));
  int pulse = eHealth.getBPM();
  int oxi = eHealth.getOxygenSaturation();
  
  outputPulse->lock();
  PulseData * outputPulseData =  outputPulse->get();
  (*outputPulseData).pulse = pulse;
  outputPulse->unlock();
  
  outputOxi->lock();
  OxiData * outputOxiData =  outputOxi->get();
  (*outputOxiData).oxi = oxi;
  outputOxi->unlock();
  
}

PulsioximeterSensor::~PulsioximeterSensor() {
  destroyPort(outputPulse);
  destroyPort(outputOxi);
}