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

#include "filters/ehealth/EMGSensor.h"
#include "filters/ehealth/tools/eHealth.h"

using namespace tmf;
using namespace std;

FilterRegister<EMGSensor> EMGSensor::reg("emg");

EMGSensor::EMGSensor(const string & name) :
Filter(name) {
  outputEMG = createOutputPort<EMGData>("EMG output");
}

void EMGSensor::init() {
  Serial.begin(115200);
  this->period = stoi(getProp("period"));
}

void EMGSensor::run() {
  
  std::this_thread::sleep_for(std::chrono::seconds(period));
  
  outputEMG->lock();
  
  EMGData * outputEMGData =  outputEMG->get();
  (*outputEMGData).emg = eHealth.getEMG();
  (*outputEMGData).t.setCurrentTime();
  outputEMG->unlock();
  
}

EMGSensor::~EMGSensor() {
  destroyPort(outputEMG);
}
