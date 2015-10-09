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
  
  Filter* temperature = Factory::createFilter("temperature", "temperature");
  Filter* pulsioximeter = Factory::createFilter("pulsioximeter", "pulsioximeter");
  Filter* display = Factory::createFilter("ehealthdisplay", "ehealthdisplay");
 
  pulsioximeter->setProp("period", 5);
  temperature->setProp("period", 2);
  
  pipe->addFilters(temperature, pulsioximeter, display, nullptr);
  
  pipe->connectFilters(pulsioximeter, display);
  pipe->connectFilters(temperature, display);
  
  pipe->init();
  
  pipe->run();
  
  Factory::destroyPipeline(pipe);
  
  return 0;
}
