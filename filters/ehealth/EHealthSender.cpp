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
  jsonHandler.data.userid = getProp("userid");
  sendingPeriod = std::stoi(getProp("sendingPeriod"));
}

void EHealthSender::run() {

  long long timediff;
  input->lock();
  SensorData * inputData = input->get();
  timediff = jsonHandler.insertData(inputData);
  input->unlock();   
  
  if (timediff >= sendingPeriod) { 
    string message = jsonHandler.toJSON();
    httpHandler.sendHTTP(message);
  }
  
}

EHealthSender::~EHealthSender() {
  destroyPort(input);
}
