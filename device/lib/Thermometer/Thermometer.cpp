#include "Thermometer.h"

Thermometer::Thermometer(uint8_t pin) : oneWire(pin), sensors(&oneWire) {
  pinMode(pin, INPUT);
}

void Thermometer::begin() {
  sensors.begin();
}

float Thermometer::getTemperature() { // return temperature in Celsius
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

/*
  < 0 °C: Very cold (freezing, dangerous without proper insulation).
  0–10 °C: Cold (recommended to dress warmly).
  10–20 °C: Cool (comfortable for most people).
  20–25 °C: Comfortable (ideal temperature for indoor and outdoor activities).
  25–30 °C: Warm (may feel hot for some, cooling might be needed).
  30–35 °C: Hot (uncomfortable without air conditioning).
  > 35 °C: Very hot (extreme temperatures, hazardous to health).
*/