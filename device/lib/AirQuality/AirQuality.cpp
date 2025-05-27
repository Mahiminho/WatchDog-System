#include "AirQuality.h"

AirQuality::AirQuality(int analogAQPin) {
  m_analogAQPin = analogAQPin;
}

void AirQuality::begin() {
  Serial.println("Initializing Air Quality Sensor...");
  analogReadResolution(12); // 12-bit ADC (0-4095)
  Serial.println("Calibrating (~10 sec)...");
  autoCalibrate(10); // calibrate for 10 seconds
  Serial.println("Air Quality Sensor Initialized.\n");
}

int AirQuality::readRaw() {
  return analogRead(m_analogAQPin);
}

float AirQuality::getResistance(int rawADC) {
  if (rawADC == 0) return 1; // avoid division by zero
  return ((4095.0 / rawADC) - 1.0) * RLOAD;
}

void AirQuality::autoCalibrate(int seconds) {
  const int samples = seconds * 10; // 10 samples per second
  uint64_t totalRaw = 0; // long

  for (int i = 0; i < samples; i++) {
    totalRaw += analogRead(m_analogAQPin);
    delay(100); // delay between reads
  }

  int avgRaw = totalRaw / samples;
  float resistance = getResistance(avgRaw);

  float ppm = 500.0; // assuming a baseline of 500 PPM for CO2 in clean air
  const float a = -0.42; // coefficients for CO2
  const float b = 1.92;
  float ratio = pow(10, (log10(ppm) - b) / a);

  RZERO = resistance / ratio; // calculate RZERO based on the average resistance
}

float AirQuality::readPPM() {
  int raw = readRaw();
  float resistance = getResistance(raw);

  const float a = -0.42;
  const float b = 1.92;

  if (RZERO == 0) return -1;

  float ratio = resistance / RZERO;

  if (ratio <= 0.0 || isnan(ratio) || isinf(ratio)) return -1;

  float ppm_log = a * log10(ratio) + b;
  float ppm = pow(10, ppm_log);

  if (isnan(ppm) || isinf(ppm) || ppm <= 0.0 || ppm > 100000.0) return -1;

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