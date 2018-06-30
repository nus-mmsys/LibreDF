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

#ifndef DF_INTEGER_H_
#define DF_INTEGER_H_

#include "core/token_type.h"

struct Integer : public df::TokenType {
 
public:
  
  Integer() { }
  
  virtual void load(char * buf) { }
  virtual char * buffer() { return nullptr; }
  
  virtual ~Integer() { }
  
};

#endif /* DF_INTEGER_H_ */
