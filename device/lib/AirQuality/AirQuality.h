#ifndef AIRQUALITY_H
#define AIRQUALITY_H

#include <Arduino.h>

class AirQuality {
public:
  AirQuality(int analogAQPin = 34); // default pin for MQ-135
  void begin();
  float readPPM();
  int readRaw();

private:
  int m_analogAQPin;
  float RZERO;
  const float RLOAD = 10.0; // load resistance in kΩ
  float getResistance(int rawADC);
  void autoCalibrate();
};

#endif // AIRQUALITY_H