#ifndef AIRQUALITY_H
#define AIRQUALITY_H

#include <Arduino.h>

class AirQuality {
public:
  AirQuality(uint8_t analogPin = 34); // default pin is 34

  void begin();
  int readRaw();
  float readPPM();

private:
  uint8_t m_analogPin;
  float m_ppmFromRaw(int raw);
};

#endif // AIRQUALITY_H