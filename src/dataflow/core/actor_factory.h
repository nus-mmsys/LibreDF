/*
 *   TMF
 *   Copyright (C) TMF Team
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

#include "actor.h"
#include <functional>

namespace df {
  
  template<typename T>
  Actor * newActor(std::string name) { 
      return new T(name); 
  }

  class ActorFactory {
  public:
    typedef std::map<std::string, std::function<Actor * (std::string)>> ActorMap;
    static Actor * createActor(std::string const& s, const std::string& name) {
      ActorMap::iterator it = actmap->find(s);
      if(it == actmap->end())
	return 0;
      return it->second(name);
    }

  private:
    static ActorMap * actmap;

  protected:
   static ActorMap * getMap() {
      // never deleted. (exist until program termination)
      // because we cannot guarantee correct destruction order 
      if(!actmap) { actmap = new ActorMap(); } 
      return actmap; 
    }
  };
  
  template<typename T>
  class ActorRegister : public ActorFactory {
  public: 
    ActorRegister(std::string const& s) { 
      getMap()->insert(make_pair(s, &newActor<T>));
    }
  };
  
}
#endif /* DF_ACTORFACTORY_H_ */
