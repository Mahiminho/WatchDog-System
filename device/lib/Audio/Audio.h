#ifndef AUDIO_H
#define AUDIO_H

#include <Arduino.h>

class Audio {
public:
  Audio(uint8_t analogPin = 32); // default pin is 32
  void begin();
  float getNoiseLevel();

private:
  uint8_t m_analogPin;
  float computeRMS();
};

#endif // AUDIO_H