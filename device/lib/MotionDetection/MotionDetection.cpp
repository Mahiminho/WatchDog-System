#include "MotionDetection.h"

MoveDetection::MoveDetection(uint8_t sensorPin) {
  m_sensorPin = sensorPin;
}

void MoveDetection::begin() {
  pinMode(m_sensorPin, INPUT);
}

bool MoveDetection::isMotionDetected() {
  return digitalRead(m_sensorPin) == HIGH;
}