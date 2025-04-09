#include "AirQuality.h"

AirQuality::AirQuality(uint8_t analogPin) {
  m_analogPin = analogPin;
}

void AirQuality::begin() {
  pinMode(m_analogPin, INPUT);
}

int AirQuality::readRaw() { // return raw ADC value (0-4095)
  return analogRead(m_analogPin);
}

float AirQuality::m_ppmFromRaw(int raw) { // convert raw ADC value to PPM (0-1000)
  return map(raw, 0, 4095, 0, 1000);
}

float AirQuality::readPPM() { // return parts per million (parts of gas per million parts of air)
  int raw = readRaw();
  return m_ppmFromRaw(raw);
}

/*
  0-100 PPM: Clean air, safe for humans (recommended as a normal level).
  100-200 PPM: Moderate pollution level, may cause discomfort for sensitive individuals (e.g., those with allergies).
  200-300 PPM: Harmful for sensitive groups, dangerous for individuals with asthma, heart conditions, or other vulnerabilities.
  300-400 PPM: Harmful to the general population; prolonged exposure should be avoided.
  400-500 PPM: Very harmful to all individuals; evacuation or significant ventilation is required.
  >500 PPM: Hazardous level requiring immediate evacuation or ventilation to prevent severe health consequences.
*/