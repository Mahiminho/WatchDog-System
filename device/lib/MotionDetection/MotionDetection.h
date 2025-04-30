#ifndef MOVEDETECTION_H
#define MOVEDETECTION_H

#include <Arduino.h>

class MoveDetection {
public:
  MoveDetection(uint8_t sensorMotionPin = 26); // default pin is 26
  void begin();
  bool isMotionDetected();

private:
  uint8_t m_sensorMotionPin;
};

#endif // MOVEDETECTION_H