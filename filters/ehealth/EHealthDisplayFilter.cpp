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

#include "filters/ehealth/EHealthDisplayFilter.h"

using namespace tmf;
using namespace std;



FilterRegister<EHealthDisplayFilter> EHealthDisplayFilter::reg("ehealthdisplay");

EHealthDisplayFilter::EHealthDisplayFilter(const string & name) :
Filter(name) {
  inputTemperature = createInputPort<TemperatureData>("temperature input");
  inputPulse = createInputPort<PulseData>("pulse input");
  inputOxi = createInputPort<OxiData>("oxi input");
  
}

void EHealthDisplayFilter::init() {
  temperature = 0;
  pulse = 0;
  oxi = 0;
}

void EHealthDisplayFilter::run() {
  
  std::thread temp = thread(&EHealthDisplayFilter::readTemperatureThread, this);
  std::thread pulsioxi = thread(&EHealthDisplayFilter::readPulsioxiThread, this);
   
  temp.join();
  pulsioxi.join();
  
}

void EHealthDisplayFilter::readTemperatureThread()
{
  while(true) {
    inputTemperature->lock();
    TemperatureData * inputTemperatureData = inputTemperature->get();
    temperature = (*inputTemperatureData).temp;
    inputTemperature->unlock();
    display();
  }
}

void EHealthDisplayFilter::readPulsioxiThread()
{
  while(true) {
    inputPulse->lock();
    PulseData * inputPulseData = inputPulse->get();
    pulse = (*inputPulseData).pulse;
    inputPulse->unlock();
    
    inputOxi->lock();
    OxiData * inputOxiData = inputOxi->get();
    oxi = (*inputOxiData).oxi;
    inputOxi->unlock();
    
    display();
  }
}

void EHealthDisplayFilter::display()
{
    unique_lock<mutex> locker(mux);
    cout << "Temperature: "  << temperature << "\n"
       << "Pulse: "  << pulse << "\n"
       << "Oxi: " << oxi << "\n"
       << "===============\n";
}







EHealthDisplayFilter::~EHealthDisplayFilter() {
  destroyPort(inputTemperature);
  destroyPort(inputPulse);
  destroyPort(inputOxi);
}