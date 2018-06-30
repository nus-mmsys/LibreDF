/*
 *   libdataflow
 *   Copyright (C) 2018
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "int_consumer.h"

using namespace df;
using namespace std;

ActorRegister<IntConsumer> IntConsumer::reg("IntConsumer");

IntConsumer::IntConsumer(const string & name) : Actor(name) {
  input = createInputPort<int>("input");
}

void IntConsumer::run() {
  int * inputData = consume(input);
  
  log("consuming "+to_string(*inputData));
  sleep(500);
  
  release(input);
}

void IntConsumer::runDist() {

  std::string msg;
  char buf[1024];
  read(input, buf, 1024);
  msg = buf;
  log("consuming "+msg);
  sleep(500);
  
}

IntConsumer::~IntConsumer() {
  destroyPort(input);
}
