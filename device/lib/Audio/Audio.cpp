#include "Audio.h"
#include <math.h>

Audio::Audio(uint8_t analogPin) {
  m_analogPin = analogPin;
}

void Audio::begin() {
  pinMode(m_analogPin, INPUT);
}

float Audio::computeRMS() { // сomputes the Root Mean Square (RMS) value of the signal; RMS is a measure of the average power of the signal
  const int samples = 1000; // number of samples to take
  unsigned long sum = 0;

  for (int i = 0; i < samples; ++i) {
    int16_t raw = analogRead(m_analogPin) - 2048; // center the signal around 0 (assuming ~2.5V reference)
    sum += raw * raw; // accumulate the square of the signal
    delayMicroseconds(50); // sampling frequency ~20kHz
  }

  return sqrt(sum / (float)samples); // calculate RMS
}

float Audio::getNoiseLevel() { // calculates the noise level in decibels (dB)
  float rms = computeRMS();

  float voltage = rms * (3.3 / 4095.0); // convert RMS to voltage (12-bit ADC, 3.3V ref)
  float dB = 20.0 * log10(voltage / 0.006); // convert voltage to dB (6mV noise floor)

  if (dB < 0) dB = 0; // clamp dB value to avoid -inf for very low values

  return dB;
}

/*
  0–20 dB: Very quiet (e.g., breathing, whispering, a quiet room at night).
  20–40 dB: Low noise (e.g., soft conversation, office environment).
  40–60 dB: Moderate noise (e.g., normal conversation, television).
  60–80 dB: Loud (e.g., street noise, traffic, vacuum cleaner).
  80–100 dB: Very loud (e.g., motorcycle, shouting, loud music).
  100–120 dB: Dangerously loud (e.g., concert, power drill).
  >120 dB: Harmful to hearing (e.g., jet engine, siren).
*/