#include "AirQuality.h"

AirQuality::AirQuality(int analogAQPin) {
  m_analogAQPin = analogAQPin;
}

void AirQuality::begin() {
  Serial.println("Initializing Air Quality Sensor...");
  analogReadResolution(12); // 12-bit ADC (0-4095)
  Serial.println("Calibrating (~5 sec)...");
  autoCalibrate();
  Serial.println("Air Quality Sensor Initialized.\n");
}

int AirQuality::readRaw() {
  return analogRead(m_analogAQPin);
}

float AirQuality::getResistance(int rawADC) {
  if (rawADC == 0) return -1; // Avoid division by zero
  return ((4095.0 / rawADC) - 1.0) * RLOAD;
}

void AirQuality::autoCalibrate() {
  const int samples = 50;
  long totalRaw = 0;

  for (int i = 0; i < samples; i++) {
    totalRaw += analogRead(m_analogAQPin);
    delay(100); // delay between reads
  }

  int avgRaw = totalRaw / samples;
  float resistance = getResistance(avgRaw);

  float ppm = 400.0;
  const float a = -0.42; // coefficients for CO2
  const float b = 1.92;
  float ratio = pow(10, (log10(ppm) - b) / a);

  RZERO = resistance / ratio;
}

float AirQuality::readPPM() {
  int raw = readRaw();
  float resistance = getResistance(raw);

  const float a = -0.42;
  const float b = 1.92;

  float ratio = resistance / RZERO;
  float ppm_log = a * log10(ratio) + b;
  float ppm = pow(10, ppm_log);

  return ppm;
}

/*
  < 400 PPM        : Unlikely in real environments – possible sensor drift or error
  400–1,000 PPM    : Normal indoor air quality
  1,000–2,000 PPM  : Drowsiness and reduced concentration may occur
  2,000–5,000 PPM  : Headaches, sleepiness, and discomfort likely
  5,000–10,000 PPM : Unsafe for prolonged exposure – risk of oxygen deprivation
  > 10,000 PPM     : Dangerous – potential for unconsciousness or death
*/