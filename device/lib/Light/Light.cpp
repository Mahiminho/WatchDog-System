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
  0-10 люксів: дуже темно (наприклад, вночі або в затемненому приміщенні без освітлення).
  10-100 люксів: низький рівень освітленості (наприклад, ввечері, коли сонце тільки заходить).
  100-500 люксів: типове приміщення з невеликим освітленням (офіс або кімната з лампами).
  500-1000 люксів: добре освітлене приміщення (наприклад, кімната з хорошим освітленням).
  1000-2000 люксів: яскраве освітлення (наприклад, на робочому столі з хорошим природним або штучним освітленням).
  2000-10000 люксів: дуже яскраве освітлення (на вулиці в сонячний день).
  >10000 люксів: дуже яскраво (пряме сонячне світло).
*/