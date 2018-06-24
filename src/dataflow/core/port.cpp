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

#include "port.h"

using namespace df;
using namespace std;

Port::Port(string name) :
name(name), rate(1), linked(0) {
}

string Port::getName() {
  return name;
}

void Port::setRate(int r) {
  rate = r;
}

int Port::getRate() {
  return rate;
}

const std::string & Port::getPortCap() const {
  
  return port_cap;
}

int Port::getLinked() {
  return linked;
}

bool Port::isConnected() {
  return linked > 0;
}

void Port::increaseLinked() {
  linked++;
}
