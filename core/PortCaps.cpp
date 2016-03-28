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

#include "core/PortCaps.h"

using namespace tmf;
using namespace std;

void PortCaps::addCaps(const string& key, const string& val) {
  
  caps.insert(make_pair(key,val));
}

const map<string, string> & PortCaps::getCaps() const { return caps; }

bool PortCaps::isEqual(const PortCaps& pc) const {
  return caps.size() == pc.getCaps().size()
  && std::equal(caps.begin(), caps.end(),
		pc.getCaps().begin());
}


