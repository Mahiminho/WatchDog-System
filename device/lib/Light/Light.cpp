#include "Light.h"

Light::Light(uint8_t address, uint8_t sdaPin, uint8_t sclPin) {
  m_address = address;
  m_sdaPin = sdaPin;
  m_sclPin = sclPin;
}

void Light::begin() {
  Serial.println("Light sensor initialization...");
  Wire.begin(m_sdaPin, m_sclPin); // init I2C with custom SDA and SCL pins
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, m_address))
    Serial.println("Light sensor initialized successfully.\n");
  else
    Serial.println("Failed to initialize light sensor!!!\n");
}

int Light::readLux() { // return light intensity in lux
  Serial.println("Reading light intensity...");
  float lux = lightMeter.readLightLevel();
  if (lux < 0) {
    Serial.println("Failed to read light intensity!!!\n");
    return -1; // error reading light intensity
  }
  Serial.println("Light intensity read.");
  return (int)lux;
}

/*
  0-10 lux: Very dark (e.g., at night or in a dimly lit room without lighting).
  10-100 lux: Low light levels (e.g., during evening hours when the sun is setting).
  100-500 lux: Typical indoor lighting (e.g., an office or room with minimal lighting).
  500-1000 lux: Well-lit indoor environment (e.g., a room with good artificial lighting).
  1000-2000 lux: Bright lighting (e.g., a workspace with strong natural or artificial light).
  2000-10000 lux: Very bright lighting (e.g., outdoors on a sunny day).
  >10000 lux: Extremely bright (e.g., direct sunlight).
*/