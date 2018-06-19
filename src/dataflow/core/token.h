/*
 *   DF
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

#ifndef DF_TOKEN_H
#define DF_TOKEN_H

#include "token_synchronizer.h"

#include <string>

namespace df {
  
  enum class TokenStatus {
    OK,
    ERROR,
    EOS
  };
  
  template <typename T>
  class Token : public TokenSynchronizer {
    
  private:
    int number;
    T * data;
    TokenStatus status;
    
  public:
    Token(): number(0), status(TokenStatus::OK) { data = new T(); } 
    
    T * get() { return data; }
    
    void setStatus(TokenStatus st) {status = st;}
    TokenStatus getStatus() {return status;}
    
    ~Token() {
      delete data;
      data = nullptr;
    }
  };
  
}
#endif // DF_TOKEN_H
