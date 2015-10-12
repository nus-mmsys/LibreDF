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

#include "filters/ehealth/ECGSensor.h"
#include "filters/ehealth/tools/eHealth.h"

using namespace tmf;
using namespace std;

FilterRegister<ECGSensor> ECGSensor::reg("ecg");

ECGSensor::ECGSensor(const string & name) :
Filter(name) {
  outputECG = createOutputPort<ECGData>("ECG output");
}

void ECGSensor::init() {
  this->period = stoi(getProp("period"));
}

void ECGSensor::run() {
  
  std::this_thread::sleep_for(std::chrono::seconds(period));
  
  outputECG->lock();
  
  ECGData * outputECGData =  outputECG->get();
  (*outputECGData).ecg = eHealth.getECG();
  (*outputECGData).t.setCurrentTime();
  outputECG->unlock();
  
}

ECGSensor::~ECGSensor() {
  destroyPort(outputECG);
}
