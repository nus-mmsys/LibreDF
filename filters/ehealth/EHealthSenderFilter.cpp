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

#include "filters/ehealth/EHealthSenderFilter.h"

using namespace tmf;
using namespace std;


FilterRegister<EHealthSenderFilter> EHealthSenderFilter::reg("ehealthsender");

EHealthSenderFilter::EHealthSenderFilter(const string& name): SensorReader(name)
{
  httphandler = new HTTPHandler("192.168.1.104");
}

void EHealthSenderFilter::process(EHealthData * data)
{
    unique_lock<mutex> locker(mux);
    httphandler->sendHTTP(data->toHTTPContent());
    std::cout << data->toHTTPContent() << '\n'
       << "===============\n";
}
