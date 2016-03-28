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

#include "core/tmf.h"

using namespace tmf;

int main(int argc, char** argv) {
  
  Pipeline* pipe = Factory::createPipeline("A math pipeline");
  
  Filter* producer = Factory::createFilter("duo_producer", "producer");
  Filter* add2 = Factory::createFilter("add2", "add2");
  Filter* multiply2 = Factory::createFilter("multiply2", "multiply2");
  Filter* duplicate = Factory::createFilter("duplicate", "duplicate");
  Filter* addition = Factory::createFilter("addition", "addition");
  
  pipe->addFilters(producer, add2, multiply2, duplicate, addition, nullptr);
  
  pipe->connectFilters(producer, multiply2);
  pipe->connectFilters(producer, add2);
  pipe->connectFilters(producer, duplicate);
  pipe->connectFilters(multiply2, addition);
  pipe->connectFilters(add2, addition);
  
  producer->setProp("limit", 10);
  
  pipe->init();
  
  pipe->run();
  
  Factory::destroyPipeline(pipe);
  
  return 0;
}
