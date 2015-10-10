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

int cont=0;

void readPulsioximeter() {
  cont++;
  if (cont==50) {
    eHealth.readPulsioximeter();
    cont = 0;
  }
}

FilterRegister<PulsioximeterSensor> PulsioximeterSensor::reg("pulsioximeter");

PulsioximeterSensor::PulsioximeterSensor(const string & name) :
Filter(name) {
  output = createOutputPort<PulsioximeterData>("output");
}

void PulsioximeterSensor::init() {
  
  this->period = stoi(getProp("period"));
  
  eHealth.initPulsioximeter();
  attachInterrupt(6, readPulsioximeter, RISING);

}

void PulsioximeterSensor::run() {
  
  std::this_thread::sleep_for(std::chrono::seconds(2));
  //delay(2000);
  int bpm = eHealth.getBPM();
  int oxygen = eHealth.getOxygenSaturation();

  if (bpm < 0 | bpm > 200 | oxygen < 0 | oxygen > 200) {
      return;
  }  

  output->lock();
  PulsioximeterData * outputPulseData =  output->get();
  (*outputPulseData).bpm = bpm;
  (*outputPulseData).oxygen = oxygen;
  output->unlock();
}

PulsioximeterSensor::~PulsioximeterSensor() {
  destroyPort(output);
}
