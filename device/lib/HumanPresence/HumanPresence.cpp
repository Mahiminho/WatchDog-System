#include "HumanPresence.h"

HumanPresence::HumanPresence(uint8_t sensorPin) {
  m_sensorPin = sensorPin;
}

void HumanPresence::begin() {
  pinMode(m_sensorPin, INPUT);
}

bool HumanPresence::isPresent() {
  return digitalRead(m_sensorPin) == HIGH;
}