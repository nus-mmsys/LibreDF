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
}

void EHealthSender::init() {
  httpHandler.setHost(getProp("host"));
  jsonHandler.userid = getProp("userid");
  sendingPeriod = std::stoi(getProp("sendingPeriod"));
}

void EHealthSender::run() {

  input->lock();
  SensorData * inputData = input->get();

  if (jsonHandler.sensordata[inputData->sensorID].timestamp == "" )
    jsonHandler.sensordata[inputData->sensorID].timestamp = std::to_string(inputData->timestamp);
  //cout << inputData->timestamp << " - " <<  jsonHandler.sensordata[inputData->sensorID].timestamp << endl; 
  map<string, string> curData;
  long long timediff =  inputData->timestamp - std::stoll(jsonHandler.sensordata[inputData->sensorID].timestamp); 
  curData[string("timediff")] = std::to_string(timediff); 
  
  switch(inputData->sensorID) {
    case 1:
      //cout << "Airflow: " << inputData->airflow << endl;
      curData[string("airflow")] = std::to_string(inputData->airflow);
      break;
    case 2:
      //cout << "Systolic: " << inputData->systolic << ", Diastolic: " << inputData->diastolic << ", Pulse: " << inputData->pulse << endl;
      curData[string("systolic")] = std::to_string(inputData->systolic);
      curData[string("diastolic")] = std::to_string(inputData->diastolic);
      curData[string("pulse")] = std::to_string(inputData->pulse);
      break;
    case 3:
      //cout << "ECG: " << inputData->ecg << endl;
      curData[string("ecg")] = std::to_string(inputData->ecg);
      break;
    case 4:
      //cout << "EMG: " << inputData->emg << endl;
      curData[string("emg")] = std::to_string(inputData->emg);
      break;
    case 5:
      //cout << "Conductance: " << inputData->conductance << ", Resistance: " << inputData->resistance << ", ConductanceVol: " << inputData->conductanceVol << endl;
      curData[string("conductance")] = std::to_string(inputData->conductance);
      curData[string("resistance")] = std::to_string(inputData->resistance);
      curData[string("conductanceVol")] = std::to_string(inputData->conductanceVol);
      break;
    case 6:
      //cout << "Glucose: " << inputData->glucose << endl;
      curData[string("glucose")] = std::to_string(inputData->glucose);
      break;
    case 7:
      //cout << "Position: " << inputData->position << endl;
      curData[string("position")] = std::to_string(inputData->position);
      break;
    case 8:
      //cout << "BPM: " << inputData->bpm << ", OxygenSaturation: " << inputData->oxygenSaturation << endl;
      curData[string("bpm")] = std::to_string(inputData->bpm);
      curData[string("oxygenSaturation")] = std::to_string(inputData->oxygenSaturation);
      break;
    case 9:
      //cout << "Temperature: " << inputData->temperature << endl;
      curData[string("temperature")] = std::to_string(inputData->temperature);
      break;
    default:
      cout << "The sensor ID does not exist.\n";
      break;
  }
  
  jsonHandler.sensordata[inputData->sensorID].data.push_back(curData);     
  input->unlock();   
  
  if (timediff >= sendingPeriod) { 
    string message = jsonHandler.toJSON();
    httpHandler.sendHTTP(message);
  }
}

EHealthSender::~EHealthSender() {
  destroyPort(input);
}
