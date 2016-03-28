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

#include "filters/ehealth/EHealthSender.h"
#include <fstream>

using namespace tmf;
using namespace std;
using namespace ehealthsensor;

FilterRegister<EHealthSender> EHealthSender::reg("ehealthsender");

EHealthSender::EHealthSender(const string & name) :
Filter(name) {
  input = createInputPort<SensorData>("sensor input");
}

void EHealthSender::init() {
  mode = std::stoi(getProp("mode"));
  httpHandler.setHost(getProp("host"));
  
  jsonHandler.setMacID();
  jsonHandler.setUserID(getProp("userid"));
  
  jsonHandler.setType(1);

  string message = jsonHandler.toJSON();
  string response = httpHandler.sendHTTP(message);
  cout << response << endl;
  
  ConfigurationManager config(response, "json"); 
  sendingPeriod = std::stoi(config.getValue("sendingperiod"));
  sessionDuration = std::stoi(config.getValue("sessionduration"));
  sessionGap = std::stoi(config.getValue("sessiongap"));
  
  gapPeriod = false;

  jsonHandler.setType(0);
  chronometer.start();
}

void EHealthSender::run() {
  
  long long timediff;
  input->lock();
  SensorData * inputData = input->get();

  if (mode==0 && gapPeriod) {
    if (chronometer.now() < sessionGap) {
      cout << "Sleeping..." << endl;
      input->unlock();
      return;
    } else {
      cout << "Waking up..." << endl;
      chronometer.start();
      gapPeriod = false;
    }
  }
  
  timediff = jsonHandler.insertData(inputData);
  input->unlock();
    
  if (timediff >= sendingPeriod) { 
    cout << "Sending..." << endl;
    string message = jsonHandler.toJSON();
    std::string response = httpHandler.sendHTTP(message);
    cout << response << endl;
  }

  if (mode==0 && chronometer.now() >= sessionDuration) {
    gapPeriod = true;
    chronometer.start();
    cout << "Session duration is up..." << endl;
  }
  
}

EHealthSender::~EHealthSender() {
  destroyPort(input);
}
