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

#ifndef PORTCAPS_H_
#define PORTCAPS_H_

#include <map>
#include <string>

using namespace std;

class PortCaps {
  
private:
  
  map<string, string> caps;

public:

  void addCaps(const string& key, const string& val) {
  
    caps.insert(make_pair(key,val));
  }
  
  const map<string, string> & getCaps() const { return caps; }

  bool isEqual(const PortCaps& pc) const {
    return caps.size() == pc.getCaps().size()
        && std::equal(caps.begin(), caps.end(),
                      pc.getCaps().begin());
  }

};


#endif /* PORTCAPS_H_ */
