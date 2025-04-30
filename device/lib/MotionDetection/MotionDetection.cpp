#include "MotionDetection.h"

MoveDetection::MoveDetection(uint8_t sensorMotionPin) {
  m_sensorMotionPin = sensorMotionPin;
}

void MoveDetection::begin() {
  Serial.println("Initializing motion sensor...");
  pinMode(m_sensorMotionPin, INPUT);
  Serial.println("Motion sensor initialized.\n");
}

bool MoveDetection::isMotionDetected() {
  Serial.println("Checking motion...");
  bool motionDetected = digitalRead(m_sensorMotionPin) == HIGH;
  Serial.println("Motion checked.");
  return motionDetected;
}