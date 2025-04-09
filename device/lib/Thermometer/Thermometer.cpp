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
  < 0 °C: дуже холодно (мороз, небезпечно без утеплення).
  0–10 °C: холодно (рекомендовано одягатися тепло).
  10–20 °C: прохолодно (зручна температура для більшості людей).
  20–25 °C: комфортно (оптимальна температура для перебування в приміщенні та на вулиці).
  25–30 °C: тепло (може бути жарко для деяких людей, потребує охолодження).
  30–35 °C: жарко (неприбуткова температура без кондиціонування).
  >35 °C: дуже жарко (екстремальні температури, небезпечно для здоров'я).
*/