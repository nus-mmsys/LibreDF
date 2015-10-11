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

#ifndef SENSORREADER_H_
#define SENSORREADER_H_

#include "core/tmf.h"
#include "core/Filter.h"
#include "core/Port.h"

#include "filters/ehealth/tools/EHealthData.h"

struct SensorReader: public tmf::Filter {
private:
  
  tmf::InputPort<AirflowData> * inputAirflow;
  tmf::InputPort<BloodPressureData> * inputBloodPressure;
  tmf::InputPort<ECGData> * inputECG;
  tmf::InputPort<EMGData> * inputEMG;
  tmf::InputPort<GalvanicData> * inputGalvanic;
  tmf::InputPort<GlucometerData> * inputGlucometer;
  tmf::InputPort<PositionData> * inputPosition;
  tmf::InputPort<PulsioximeterData> * inputPulsioximeter;
  tmf::InputPort<TemperatureData> * inputTemperature;
  
  static tmf::FilterRegister<SensorReader> reg;

protected:

  AirflowData airflow;
  BloodPressureData bloodpressure;
  ECGData ecg;
  EMGData emg;
  GalvanicData galvanic;
  GlucometerData glucometer;
  PositionData position;
  PulsioximeterData pulsioximeter;
  TemperatureData temperature;
  
public:
  
  SensorReader(const std::string& name);
  
  virtual void init();
  
  virtual void run();
  
  virtual void process() {}

  void readAirflowThread();
  void readBloodPressureThread();
  void readECGThread();
  void readEMGThread();
  void readGalvanicThread();
  void readGlucometerThread();
  void readPositionThread();
  void readPulsioximeterThread();
  void readTemperatureThread();
  
  virtual ~SensorReader();
  
};

#endif /* SENSORREADER_H_ */
