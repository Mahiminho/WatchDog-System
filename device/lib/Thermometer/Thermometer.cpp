#include "Thermometer.h"

Thermometer::Thermometer(uint8_t analogTempPin) : oneWire(analogTempPin), sensors(&oneWire) {
  pinMode(analogTempPin, INPUT);
}

void Thermometer::begin() {
  Serial.println("Initializing thermometer...");
  sensors.begin();
  Serial.println("Thermometer initialized.\n");
}

float Thermometer::getTemperature() { // return temperature in Celsius
  Serial.println("Requesting temperature...");
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  temperature = round(temperature * 100.0) / 100.0; // Limit to 2 decimal places
  Serial.println("Temperature received.");
  return temperature;
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