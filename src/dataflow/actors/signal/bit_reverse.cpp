/*
 *   TMF
 *   Copyright (C) TMF Team
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

#include "bit_reverse.h"

using namespace df;
using namespace std;

ActorRegister<BitReverse> BitReverse::reg("BitReverse");

BitReverse::BitReverse(const string & name) : Actor(name) {
  input = createInputPort<ComplexVector>("input");
}

void BitReverse::init() {

}

void BitReverse::run() {
  ComplexVector * in = consume(input);

  for (int ii = 0, j = 1; j < in->vector_size()-1; j++) {
        for (int k = in->vector_size() >> 1; k > (ii ^= k); k >>= 1);
        if (ii < j) in->swap_item(ii, j);
  }

  msg = "consuming \n";
  for (int k=0; k<in->vector_size(); k++) {
	  item = in->get_item(k);
	  msg += to_string(item.real())+","+to_string(item.imag())+"i  ";
  }
  log(msg);
  
  //timer.randSleep(200);
  
  release(input);
}

BitReverse::~BitReverse() {
  destroyPort(input);
}
