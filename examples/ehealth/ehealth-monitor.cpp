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
  
  Filter* producerT = Factory::createFilter("temperature", "temperature");
  Filter* producerP = Factory::createFilter("pulsioximeter", "pulsioximeter");
  Filter* consumer = Factory::createFilter("ehealthdisplay", "ehealthdisplay");
 
  producerP->setProp("period", 5);
  producerT->setProp("period", 2);
  
  pipe->addFilters(producerT, producerP, consumer, nullptr);
  
  pipe->connectFilters(producerP, consumer);
  pipe->connectFilters(producerP, consumer);
  pipe->connectFilters(producerT, consumer);
  
  pipe->init();
  
  pipe->run();
  
  Factory::destroyPipeline(pipe);
  
  return 0;
}
