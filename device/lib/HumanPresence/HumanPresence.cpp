#include "HumanPresence.h"

HumanPresence::HumanPresence(uint8_t sensorPresencePin) {
  m_sensorPresencePin = sensorPresencePin;
}

void HumanPresence::begin() {
  Serial.println("Initializing human presence sensor...");
  pinMode(m_sensorPresencePin, INPUT);
  Serial.println("Human presence sensor initialized.\n");
}

bool HumanPresence::isPresent() {
  Serial.println("Checking human presence...");
  bool presenceDetected = digitalRead(m_sensorPresencePin) == HIGH;
  Serial.println("Human presence checked.");
  return presenceDetected;
}