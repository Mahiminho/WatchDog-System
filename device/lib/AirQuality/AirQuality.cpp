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
  0-100 PPM: чисте повітря, безпечне для людей (рекомендується як нормальний рівень).
  100-200 PPM: помірний рівень забруднення, може бути некомфортно для чутливих осіб (наприклад, алергіків).
  200-300 PPM: шкідливо для чутливих груп, небезпечно для людей з астмою, серцевими захворюваннями та інших чутливих груп.
  300-400 PPM: шкідливо для загальної популяції, слід уникати тривалого перебування в такому повітрі.
  400-500 PPM: дуже шкідливо для всіх людей, вимагає евакуації або серйозної вентиляції.
  >500 PPM: небезпечний рівень, який потребує негайної евакуації або вентиляції для запобігання серйозним наслідкам для здоров'я.
*/