#ifndef LIGHT_H
#define LIGHT_H

#include <Wire.h>

class Light {
public:
  Light(uint8_t address = 0x23, uint8_t sdaPin = 21, uint8_t sclPin = 22); // default I2C address is 0x23, SDA pin is 21, SCL pin is 22
  void begin();
  float readLux();

private:
  uint8_t m_address;
  uint8_t m_sdaPin;
  uint8_t m_sclPin;
};

#endif // LIGHT_H