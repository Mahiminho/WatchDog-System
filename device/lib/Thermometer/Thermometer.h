#ifndef THERMOMETER_H
#define THERMOMETER_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

class Thermometer {
public:
  Thermometer(uint8_t analogTempPin = 4); // default pin is 4
  void begin();
  float getTemperature();

private:
  OneWire oneWire;
  DallasTemperature sensors;
};

#endif // THERMOMETER_H