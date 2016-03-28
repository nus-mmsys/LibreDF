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

#ifndef MULTIPLY2FILTER_H_
#define MULTIPLY2FILTER_H_

#include "core/tmf.h"
#include "core/Filter.h"


class Multiply2Filter: public tmf::Filter {
private:
  tmf::InputPort<int> * input;
  tmf::OutputPort<int> * output;
  
  static tmf::FilterRegister<Multiply2Filter> reg;
public:
  
  Multiply2Filter(const std::string & name);
  
  virtual void run();
  
  virtual ~Multiply2Filter();
  
};

#endif /* MULTIPLY2FILTER_H_ */
