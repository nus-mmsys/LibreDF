/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2015  <copyright holder> <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "EHealthData.h"

EHealthData::EHealthData()
{

}

EHealthData::EHealthData(const EHealthData& other)
{

}

std::string EHealthData::positionToString( uint8_t position ) {
  
  switch(position) {
    case 1:
      return std::string("Prone position");
    case 2:
      return std::string("Stand or sit position");
    case 3:
      return std::string("Left lateral decubitus");
    case 4:
      return std::string("Supine position");
    case 5:
      return std::string("Rigth lateral decubitus");
    default:
      return std::string("non-defined position");
  }
}

EHealthData::~EHealthData()
{

}
