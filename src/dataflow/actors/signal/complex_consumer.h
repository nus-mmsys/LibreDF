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

#ifndef DF_COMPLEXCONSUMER_H_
#define DF_COMPLEXCONSUMER_H_

#include "core/df.h"
#include "tokens/basic/complex_vector.h"

#include <iostream>

class ComplexConsumer: public df::Actor {
  
private:
  std::string msg;
  std::complex<double> item;
  df::InputPort<df::ComplexVector> * input;

  static df::ActorRegister<ComplexConsumer> reg;
public:
  
  ComplexConsumer(const std::string & name);
  
  virtual void init();
  virtual void run();
  
  virtual ~ComplexConsumer();
  
};

#endif /* DF_COMPLEXCONSUMER_H_ */
