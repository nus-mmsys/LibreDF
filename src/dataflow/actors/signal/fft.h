/*
 *   LibreDF
 *   Copyright (C) A. Shafiei
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

#ifndef DF_FFT_H_
#define DF_FFT_H_

#include "core/df.h"
#include "tokens/basic/int.h"
#include "tokens/basic/complex_vector.h"

#include <iostream>
#include <string>
#include <unistd.h>
#include <cmath>
#include <complex>

using namespace std;

const double pi = std::acos(-1);
const std::complex<double> i(0,1);

class FFT: public df::Actor {
  
private:
  
  df::InputPort<df::ComplexVector> * input;
  df::OutputPort<df::ComplexVector> * output1;
  df::OutputPort<df::ComplexVector> * output2;
 
  static  df::ActorRegister<FFT> reg;
public:
  
  FFT(const string& name);
  
  virtual void init();
  
  virtual void run();
  
  virtual ~FFT();
  
};

#endif /* DF_FFT_H_ */
