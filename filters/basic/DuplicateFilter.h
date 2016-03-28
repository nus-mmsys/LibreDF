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

#ifndef DUPLICATEFILTER_H_
#define DUPLICATEFILTER_H_

#include "core/Filter.h"
#include "core/tmf.h"

#include <unistd.h>
#include <iostream>

struct DuplicateFilter: public tmf::Filter {
  
private:
  tmf::InputPort<std::string> * input;

  static tmf::FilterRegister<DuplicateFilter> reg;
public:
  
  DuplicateFilter(const std::string & name);
  
  virtual void run();
  
  virtual ~DuplicateFilter();
  
};

#endif /* DUPLICATEFILTER_H_ */
