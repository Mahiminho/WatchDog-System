#ifndef AUDIO_H
#define AUDIO_H

#include <Arduino.h>

class Audio {
public:
  Audio(uint8_t analogAudioPin = 32); // default pin is 32
  void begin();
  float getNoiseLevel();

private:
  uint8_t m_analogAudioPin;
  float computeRMS();
};

#endif // AUDIO_H