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
  inputPulsioximeter = createInputPort<PulsioximeterData>("pulsioximeter input");
  
}

void EHealthDisplayFilter::init() {
  temperature = 0;
  bpm = 0;
  oxygen = 0;
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
    temperature = (*inputTemperatureData).temperature;
    inputTemperature->unlock();
    display();
  }
}

void EHealthDisplayFilter::readPulsioxiThread()
{
  while(true) {
    inputPulsioximeter->lock();
    PulsioximeterData * inputPulsioxiData = inputPulsioximeter->get();
    bpm = (*inputPulsioxiData).bpm;
    oxygen = (*inputPulsioxiData).oxygen;
    inputPulsioximeter->unlock();
    
    display();
  }
}

void EHealthDisplayFilter::display()
{
    unique_lock<mutex> locker(mux);
    cout << "Temperature: "  << temperature << "\n"
       << "BPM: "  << bpm << "\n"
       << "Oxygen: " << oxygen << "\n"
       << "===============\n";
}


EHealthDisplayFilter::~EHealthDisplayFilter() {
  destroyPort(inputTemperature);
  destroyPort(inputPulsioximeter);
}
