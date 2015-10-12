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
  
  std::thread airflowThread;
  std::thread bloodPressureThread;
  std::thread ecgThread;
  std::thread emgThread;
  std::thread galvanicThread;
  std::thread glucometerThread;
  std::thread positionThread;
  std::thread pulsioximeterThread;
  std::thread temperatureThread;
  
  if (inputAirflow->isConnected())
    airflowThread = thread(&SensorReader::readAirflowThread, this);
  
  if (inputBloodPressure->isConnected())
    bloodPressureThread = thread(&SensorReader::readBloodPressureThread, this);
  
  if (inputECG->isConnected())
    ecgThread = thread(&SensorReader::readECGThread, this);
  
  if (inputEMG->isConnected())
    emgThread = thread(&SensorReader::readEMGThread, this);
  
  if (inputGalvanic->isConnected())
    galvanicThread = thread(&SensorReader::readGalvanicThread, this);
  
  if (inputGlucometer->isConnected())
    glucometerThread = thread(&SensorReader::readGlucometerThread, this);
  
  if (inputPosition->isConnected())
    positionThread = thread(&SensorReader::readPositionThread, this);
  
  if (inputPulsioximeter->isConnected())
    pulsioximeterThread = thread(&SensorReader::readPulsioximeterThread, this);
  
  if (inputTemperature->isConnected())
    temperatureThread = thread(&SensorReader::readTemperatureThread, this);

  
  if (inputAirflow->isConnected())
    airflowThread.join();
  if (inputBloodPressure->isConnected())
    bloodPressureThread.join();
  if (inputECG->isConnected())
    ecgThread.join();
  if (inputEMG->isConnected())
    emgThread.join();
  if (inputGalvanic->isConnected())
    galvanicThread.join();
  if (inputGalvanic->isConnected())
    glucometerThread.join();
  if (inputPosition->isConnected())
    positionThread.join();
  if (inputPulsioximeter->isConnected())
    pulsioximeterThread.join();
  if (inputTemperature->isConnected())
    temperatureThread.join(); 
}

void SensorReader::readAirflowThread()
{
  while(true) {
    inputAirflow->lock();
    AirflowData * airflowData = inputAirflow->get();
    process(airflowData);
    inputAirflow->unlock();

  }
}

void SensorReader::readBloodPressureThread()
{
  while(true) {
    inputBloodPressure->lock();
    BloodPressureData * bloodPressureData = inputBloodPressure->get();
    process(bloodPressureData);
    inputBloodPressure->unlock();
  }
}

void SensorReader::readECGThread()
{
  while(true) {
    inputECG->lock();
    ECGData * ecgData = inputECG->get();
    process(ecgData);
    inputECG->unlock();
  }
}

void SensorReader::readEMGThread()
{
  while(true) {
    inputEMG->lock();
    EMGData * emgData = inputEMG->get();
    process(emgData);
    inputEMG->unlock();
  }
}

void SensorReader::readGalvanicThread()
{
  while(true) {
    inputGalvanic->lock();
    GalvanicData * galvanicData = inputGalvanic->get();
    process(galvanicData);
    inputGalvanic->unlock();
  }
}

void SensorReader::readGlucometerThread()
{
  while(true) {
    inputGlucometer->lock();
    GlucometerData * glucometerData = inputGlucometer->get();
    process(glucometerData);
    inputGlucometer->unlock();
  }
}

void SensorReader::readPositionThread()
{
  while(true) {
    inputPosition->lock();
    PositionData * positionData = inputPosition->get();
    process(positionData);
    inputPosition->unlock();
  }
}

void SensorReader::readTemperatureThread()
{
  while(true) {
    inputTemperature->lock();
    TemperatureData * temperatureData = inputTemperature->get();
    process(temperatureData);
    inputTemperature->unlock();
  }
}

void SensorReader::readPulsioximeterThread()
{
  while(true) {
    inputPulsioximeter->lock();
    PulsioximeterData * pulsioximeterData = inputPulsioximeter->get();
    process(pulsioximeterData);
    inputPulsioximeter->unlock();
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