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

#ifndef DF_ACTORFACTORY_H_
#define DF_ACTORFACTORY_H_

#include <string>
#include <ctime>

#include "actor.h"

namespace df {
  
  template<typename T>
  Actor * createActorT(std::string name) { return new T(name); }
  
  struct ActorFactory {
    
    typedef std::map<std::string, std::function<Actor * (std::string)>> map_type;
    static Actor * createActor(std::string const& s, const std::string& name) {
      map_type::iterator it = mymap->find(s);
      if(it == mymap->end())
	return 0;
      return it->second(name);
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
  struct ActorRegister : ActorFactory { 
    ActorRegister(std::string const& s) { 
      getMap()->insert(make_pair(s, &createActorT<T>));
    }
  };
  
}
#endif /* DF_ACTORFACTORY_H_ */
