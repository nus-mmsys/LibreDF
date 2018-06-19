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

#ifndef DF_BUS_H
#define DF_BUS_H

#include <vector>

#include "attribute.h"
#include "event.h"
#include "event_observer.h"

namespace df {
  
  class Bus
  {
    
  private:
    std::vector<EventObserver *> observers;
    
  public:
    void registerObserver(EventObserver * obs);
    void notify(Event event);
  };
  
}

#endif // DF_BUS_H
