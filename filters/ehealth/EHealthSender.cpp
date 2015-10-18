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

#include "filters/ehealth/EHealthSender.h"

using namespace tmf;
using namespace std;
using namespace ehealthsensor;

FilterRegister<EHealthSender> EHealthSender::reg("ehealthsender");

EHealthSender::EHealthSender(const string & name) :
Filter(name) {
  input = createInputPort<SensorData>("sensor input");
  minPeriod = 50;
  maxPeriod = 20000;
}

void EHealthSender::init() {
  jsonHandler.userid = getProp("userid");
  sendingPeriod = std::stoi(getProp("sendingPeriod"));
}

void EHealthSender::run() {
  input->lock();
  SensorData * inputData = input->get();
  cout << inputData->timestamp << ": ";
  switch(inputData->sensorID) {
    case 1:
      //jsonHandler.sensordata[inputData->sensorID]
      cout << "Airflow: " << inputData->airflow << endl;
      break;
    case 2:
      cout << "Systolic: " << inputData->systolic << ", Diastolic: " << inputData->diastolic << ", Pulse: " << inputData->pulse << endl;
      break;
    case 3:
      cout << "ECG: " << inputData->ecg << endl;
      break;
    case 4:
      cout << "EMG: " << inputData->emg << endl;
      break;
    case 5:
      cout << "Conductance: " << inputData->conductance << ", Resistance: " << inputData->resistance << ", ConductanceVol: " << inputData->conductanceVol << endl;
      break;
    case 6:
      cout << "Glucose: " << inputData->glucose << endl;
      break;
    case 7:
      cout << "Position: " << inputData->position << endl;
      break;
    case 8:
      cout << "BPM: " << inputData->bpm << ", OxygenSaturation: " << inputData->oxygenSaturation << endl;
      break;
    case 9:
      cout << "Temperature: " << inputData->temperature << endl;
      break;
    default:
      cout << "The sensor ID does not exist.\n";
      break;
  }
  
  input->unlock();   
}

EHealthSender::~EHealthSender() {
  destroyPort(input);
}
