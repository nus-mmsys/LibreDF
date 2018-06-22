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

#ifndef DF_H_
#define DF_H_

#include <string>
#include <ctime>

#include "dataflow.h"
#include "actor.h"

namespace df {
  
  template<typename T>
  Actor * createActorT(std::string name) { return new T(name); }
  
  struct Factory {
    
    typedef std::map<std::string, std::function<Actor * (std::string)>> map_type;
    static Actor * createActor(std::string const& s, const std::string& name) {
      map_type::iterator it = mymap->find(s);
      if(it == mymap->end())
	return 0;
      return it->second(name);
    }
    
    static Dataflow * createDataflow(const std::string& name) {
      return new Dataflow(name);
    }
    
    static void destroyDataflow(Dataflow * dataflow) {
      delete dataflow; 
    }
    
  protected:
    static map_type * getMap() {
      // never delete'ed. (exist until program termination)
      // because we can't guarantee correct destruction order 
      if(!mymap) { mymap = new map_type(); } 
      return mymap; 
    }
    
  private:
    static map_type * mymap;
    
  };
  
  template<typename T>
  struct ActorRegister : Factory { 
    ActorRegister(std::string const& s) { 
      getMap()->insert(make_pair(s, &createActorT<T>));
    }
  };
  
}
#endif /* DF_H_ */
