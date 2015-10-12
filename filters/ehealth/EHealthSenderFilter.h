/*
 * 
 *  Tiny Multimedia Framework
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef EHEALTHSENDERFILTER_H_
#define EHEALTHSENDERFILTER_H_


#include "filters/ehealth/SensorReader.h"
#include "filters/ehealth/tools/EHealthData.h"


struct EHealthSenderFilter: public SensorReader {
private:
  
  static tmf::FilterRegister<EHealthSenderFilter> reg;
  
  std::mutex mux;
  
public:
  
  EHealthSenderFilter(const std::string& name);
  
  virtual void process(EHealthData data);
  
};

#endif /* EHEALTHSENDERFILTER_H_ */
