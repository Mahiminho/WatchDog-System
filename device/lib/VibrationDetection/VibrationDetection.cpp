#include "VibrationDetection.h"

VibrationDetection::VibrationDetection(uint8_t sensorPin) {
  m_sensorPin = sensorPin;
}

void VibrationDetection::begin() {
  pinMode(m_sensorPin, INPUT);
}

bool VibrationDetection::isVibrationDetected() {
  return digitalRead(m_sensorPin) == HIGH;
}