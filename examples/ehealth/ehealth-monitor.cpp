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

#include "core/tmf.h"

using namespace tmf;

int main(int argc, char** argv) {
  
  Pipeline* pipe = Factory::createPipeline("E-Health Monitor");
  
//  Filter* airflow = Factory::createFilter("airflow", "airflow");
//  Filter* bloodpressure = Factory::createFilter("bloodpressure", "bloodpressure");
//  Filter* ecg = Factory::createFilter("ecg", "ecg");
//  Filter* emg = Factory::createFilter("emg", "emg");
//  Filter* galvanic = Factory::createFilter("galvanic", "galvanic");
//  Filter* glucometer = Factory::createFilter("glucometer", "glucometer");
//  Filter* position = Factory::createFilter("position", "position");
  Filter* pulsioximeter = Factory::createFilter("pulsioximeter", "pulsioximeter");
//  Filter* temperature = Factory::createFilter("temperature", "temperature");
  Filter* display = Factory::createFilter("ehealthdisplay", "ehealthdisplay");

//  airflow->setProp("period", 2);
//  bloodpressure->setProp("period", 2);
//  ecg->setProp("period", 2);
//  emg->setProp("period", 2);
//  galvanic->setProp("period", 2);
//  glucometer->setProp("period", 2);
//  position->setProp("period", 2);
    pulsioximeter->setProp("period", 2);
//  temperature->setProp("period", 2);
  
  pipe->addFilters(/*airflow, bloodpressure, ecg, emg, galvanic, glucometer, position, temperature,*/ pulsioximeter, display, nullptr);

//  pipe->connectFilters(airflow, display);
//  pipe->connectFilters(bloodpressure, display);
//  pipe->connectFilters(ecg, display);
//  pipe->connectFilters(emg, display);
//  pipe->connectFilters(galvanic, display);
//  pipe->connectFilters(glucometer, display);
//  pipe->connectFilters(position, display);
//  pipe->connectFilters(temperature, display);
  pipe->connectFilters(pulsioximeter, display);

  pipe->init();
  
  pipe->run();
  
  Factory::destroyPipeline(pipe);
  
  return 0;
}
