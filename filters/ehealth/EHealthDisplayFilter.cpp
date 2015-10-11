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

#include "filters/ehealth/EHealthDisplayFilter.h"

using namespace tmf;
using namespace std;


FilterRegister<EHealthDisplayFilter> EHealthDisplayFilter::reg("ehealthdisplay");


void EHealthDisplayFilter::process()
{
    unique_lock<mutex> locker(mux);
    std::cout << "Airflow: "  << airflow.airflow << "\n"
       << "Bloodpressure, Systolic: "  << bloodpressure.systolic << " mmHg\n"
       << "Bloodpressure, Diastolic: " << bloodpressure.diastolic << " mmHg\n"
       << "Bloodpressure, Pulse: "  << bloodpressure.pulse << " bpm\n"
       << "ECG: " << ecg.ecg << "\n"
       << "EMG: "  << emg.emg << "\n"
       << "Galvanic, Conductance: " << galvanic.conductance << "\n"
       << "Galvanic, Resistance: "  << galvanic.resistance << "\n"
       << "Galvanic, Conductance Voltage: " << galvanic.voltage << "\n"
       << "Glucometer, Glucose: "  << glucometer.glucose << "\n"
       << "Position: " << EHealthData::positionToString(position.position) << "\n"
       << "PRBpm: "  << pulsioximeter.bpm << "\n"
       << "SPo2: "  << pulsioximeter.oxygen << "\n"
       << "Temperature: "  << temperature.temperature << "\n"
       << "===============\n";
}