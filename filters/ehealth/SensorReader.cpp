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

#include "filters/ehealth/SensorReader.h"

using namespace tmf;
using namespace std;


FilterRegister<SensorReader> SensorReader::reg("reader");

SensorReader::SensorReader(const string & name) :
Filter(name) {
  inputAirflow = createInputPort<AirflowData>("airflow input");
  inputBloodPressure = createInputPort<BloodPressureData>("bloodpressure input");
  inputECG = createInputPort<ECGData>("ecg input");
  inputEMG = createInputPort<EMGData>("emg input");
  inputGalvanic = createInputPort<GalvanicData>("galvanic input");
  inputGlucometer = createInputPort<GlucometerData>("glucometer input");
  inputPosition = createInputPort<PositionData>("position input");
  inputPulsioximeter = createInputPort<PulsioximeterData>("pulsioximeter input");
  inputTemperature = createInputPort<TemperatureData>("temperature input");
}

void SensorReader::init() {
  airflow.airflow = 0;
  bloodpressure.systolic = 0;
  bloodpressure.diastolic = 0;
  bloodpressure.pulse = 0;
  ecg.ecg = 0;
  emg.emg = 0;
  galvanic.conductance = 0;
  galvanic.resistance = 0;
  galvanic.voltage = 0;
  glucometer.glucose = 0;
  position.position = -1;
  pulsioximeter.bpm = 0;
  pulsioximeter.oxygen = 0;  
  temperature.temperature = 0;
}

void SensorReader::run() {
  
  std::thread airflowThread = thread(&SensorReader::readAirflowThread, this);
  std::thread bloodPressureThread = thread(&SensorReader::readBloodPressureThread, this);
  std::thread ecgThread = thread(&SensorReader::readECGThread, this);
  std::thread emgThread = thread(&SensorReader::readEMGThread, this);
  std::thread galvanicThread = thread(&SensorReader::readGalvanicThread, this);
  std::thread glucometerThread = thread(&SensorReader::readGlucometerThread, this);
  std::thread positionThread = thread(&SensorReader::readPositionThread, this);
  std::thread pulsioximeterThread = thread(&SensorReader::readPulsioximeterThread, this);
  std::thread temperatureThread = thread(&SensorReader::readTemperatureThread, this);

  airflowThread.join();
  bloodPressureThread.join();
  ecgThread.join();
  emgThread.join();
  galvanicThread.join();
  glucometerThread.join();
  positionThread.join();
  pulsioximeterThread.join();
  temperatureThread.join(); 
}

void SensorReader::readAirflowThread()
{
  while(true) {
    inputAirflow->lock();
    AirflowData * airflowData = inputAirflow->get();
    airflow = *airflowData;
    inputAirflow->unlock();
    process();
  }
}

void SensorReader::readBloodPressureThread()
{
  while(true) {
    inputBloodPressure->lock();
    BloodPressureData * bloodPressureData = inputBloodPressure->get();
    bloodpressure = *bloodPressureData;
    inputBloodPressure->unlock();
    process();
  }
}

void SensorReader::readECGThread()
{
  while(true) {
    inputECG->lock();
    ECGData * ecgData = inputECG->get();
    ecg = *ecgData;
    inputECG->unlock();
    process();
  }
}

void SensorReader::readEMGThread()
{
  while(true) {
    inputEMG->lock();
    EMGData * emgData = inputEMG->get();
    emg = *emgData;
    inputEMG->unlock();
    process();
  }
}

void SensorReader::readGalvanicThread()
{
  while(true) {
    inputGalvanic->lock();
    GalvanicData * galvanicData = inputGalvanic->get();
    galvanic = *galvanicData;
    inputGalvanic->unlock();
    process();
  }
}

void SensorReader::readGlucometerThread()
{
  while(true) {
    inputGlucometer->lock();
    GlucometerData * glucometerData = inputGlucometer->get();
    glucometer = *glucometerData;
    inputGlucometer->unlock();
    process();
  }
}

void SensorReader::readPositionThread()
{
  while(true) {
    inputPosition->lock();
    PositionData * positionData = inputPosition->get();
    position = *positionData;
    inputPosition->unlock();
    process();
  }
}

void SensorReader::readTemperatureThread()
{
  while(true) {
    inputTemperature->lock();
    TemperatureData * temperatureData = inputTemperature->get();
    temperature = *temperatureData;
    inputTemperature->unlock();
    process();
  }
}

void SensorReader::readPulsioximeterThread()
{
  while(true) {
    inputPulsioximeter->lock();
    PulsioximeterData * pulsioximeterData = inputPulsioximeter->get();
    pulsioximeter = *pulsioximeterData;
    inputPulsioximeter->unlock();
    process();
  }
}

SensorReader::~SensorReader() {
  destroyPort(inputAirflow);
  destroyPort(inputBloodPressure);
  destroyPort(inputECG);
  destroyPort(inputEMG);
  destroyPort(inputGalvanic);
  destroyPort(inputGlucometer);
  destroyPort(inputPosition);
  destroyPort(inputPulsioximeter);
  destroyPort(inputTemperature);
}
