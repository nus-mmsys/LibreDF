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

#ifndef EHEALTHDISPLAYFILTER_H_
#define EHEALTHDISPLAYFILTER_H_

#include "core/tmf.h"
#include "core/Filter.h"
#include "core/Port.h"

#include "filters/ehealth/tools/EHealthData.h"

struct EHealthDisplayFilter: public tmf::Filter {
private:
  tmf::InputPort<TemperatureData> * inputTemperature;
  tmf::InputPort<PulseData> * inputPulse;
  tmf::InputPort<OxiData> * inputOxi;
  
  static tmf::FilterRegister<EHealthDisplayFilter> reg;
  
  float temperature;
  int pulse, oxi;
  
  std::mutex mux;
  
public:
  
  EHealthDisplayFilter(const std::string& name);
  
  virtual void init();
  
  virtual void run();
  
  void readTemperatureThread();
  void readPulsioxiThread();
  void display();
  
  virtual ~EHealthDisplayFilter();
  
};

#endif /* EHEALTHDISPLAYFILTER_H_ */
