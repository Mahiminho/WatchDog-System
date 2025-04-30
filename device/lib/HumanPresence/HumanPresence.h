#ifndef HUMANPRESENCE_H
#define HUMANPRESENCE_H

#include <Arduino.h>

class HumanPresence {
public:
  HumanPresence(uint8_t sensorPresencePin = 27); // default pin is 27
  void begin();
  bool isPresent();

private:
  uint8_t m_sensorPresencePin;
};

#endif // HUMANPRESENCE_H