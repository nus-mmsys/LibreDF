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

#ifndef FILTERFACTORY_H_
#define FILTERFACTORY_H_

#include <string>

#include "core/Pipeline.h"
#include "core/Filter.h"
#include "tools/ConfigurationManager.h"

namespace tmf {
  
  template<typename T>
  Filter * createFilterT(std::string name) { return new T(name); }
  
  struct Factory {
    
    typedef std::map<std::string, std::function<Filter * (std::string)>> map_type;
    static Filter * createFilter(std::string const& s, const std::string& name) {
      map_type::iterator it = mymap->find(s);
      if(it == mymap->end())
	return 0;
      return it->second(name);
    }
    
    static Pipeline * createPipeline(const std::string& name) {
      return new Pipeline(name);
    }
    
    static void destroyPipeline(Pipeline * pipe) {
      delete pipe; 
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
  struct FilterRegister : Factory { 
    FilterRegister(std::string const& s) { 
      getMap()->insert(make_pair(s, &createFilterT<T>));
    }
  };
  
}
#endif /* FILTERFACTORY_H_ */
