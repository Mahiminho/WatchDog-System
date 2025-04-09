#include "Light.h"

Light::Light(uint8_t address, uint8_t sdaPin, uint8_t sclPin) {
  m_address = address;
  m_sdaPin = sdaPin;
  m_sclPin = sclPin;
}

void Light::begin() {
  Wire.begin(m_sdaPin, m_sclPin); // init I2C with custom SDA and SCL pins
  Wire.beginTransmission(m_address);
  Wire.write(0x01); // set sensor to continuous measurement mode
  Wire.endTransmission();
}

float Light::readLux() { // return light intensity in lux
  uint8_t data[2];

  Wire.beginTransmission(m_address);
  Wire.requestFrom(m_address, (uint8_t)2);
  if (Wire.available() == 2) {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  Wire.endTransmission();

  uint16_t rawData = (data[0] << 8) | data[1];
  float lux = rawData / 1.2;

  return lux;
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