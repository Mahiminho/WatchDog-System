#ifndef LIGHT_H
#define LIGHT_H

#include <Wire.h>
#include <BH1750.h>

class Light {
public:
  Light(uint8_t address = 0x23, uint8_t sdaPin = 21, uint8_t sclPin = 22); // default I2C address is 0x23, SDA pin is 21, SCL pin is 22
  void begin();
  int readLux();

private:
  uint8_t m_address;
  uint8_t m_sdaPin;
  uint8_t m_sclPin;
  BH1750 lightMeter;
};

#endif // LIGHT_H