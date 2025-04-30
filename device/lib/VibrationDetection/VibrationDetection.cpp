#include "VibrationDetection.h"

VibrationDetection::VibrationDetection(uint8_t vibrationSensorPin) {
  m_vibrationSensorPin = vibrationSensorPin;
}

void VibrationDetection::begin() {
  Serial.println("Initializing vibration sensor...");
  pinMode(m_vibrationSensorPin, INPUT);
  Serial.println("Vibration sensor initialized.\n");
}

bool VibrationDetection::isVibrationDetected() {
  Serial.println("Checking vibration...");
  bool vibrationDetected = digitalRead(m_vibrationSensorPin) == HIGH;
  Serial.println("Vibration checked.\n");
  return vibrationDetected;
}