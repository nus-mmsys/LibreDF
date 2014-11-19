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

#include "tmf.h"


int main(int argc, char** argv) {
  
  TMF tmf;
  
  Pipeline* pipe = tmf.createPipeline("A math pipeline");
  
  Filter* producer = tmf.createFilter(DOUBLEPRODUCER_FILTER, "producer");
  Filter* add2 = tmf.createFilter(ADD2_FILTER, "add2");
  Filter* multiply2 = tmf.createFilter(MULTIPLY2_FILTER, "multiply2");
  Filter* duplicate = tmf.createFilter(DUPLICATE_FILTER, "duplicate");
  Filter* addition = tmf.createFilter(ADDITION_FILTER, "addition");
  
  pipe->connectFilters(producer, multiply2);
  pipe->connectFilters(producer, add2);
  pipe->connectFilters(producer, duplicate);
  pipe->connectFilters(multiply2, addition);
  pipe->connectFilters(add2, addition);
  
  producer->setProp("limit", 10);
  
  pipe->init();
  
  pipe->run();
  
  tmf.destroyPipeline(pipe);
  
  return 0;
}
