#ifndef VIBRATIONDETECTION_H
#define VIBRATIONDETECTION_H

#include <Arduino.h>

class VibrationDetection {
public:
  VibrationDetection(uint8_t vibrationSensorPin = 12); // default pin is 12
  void begin();
  bool isVibrationDetected();

private:
  uint8_t m_vibrationSensorPin;
};

#endif // VIBRATIONDETECTION_H