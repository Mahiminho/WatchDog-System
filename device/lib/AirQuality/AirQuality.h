#ifndef AIRQUALITY_H
#define AIRQUALITY_H

#include <Arduino.h>

class AirQuality {
public:
  AirQuality(uint8_t analogAQPin = 34); // default pin is 34
  void begin();
  int readRaw();
  float readPPM();

private:
  uint8_t m_analogAQPin;
  float m_ppmFromRaw(int raw);
};

#endif // AIRQUALITY_H