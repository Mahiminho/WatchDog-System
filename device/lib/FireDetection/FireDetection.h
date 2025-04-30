#ifndef FIRE_DETECTION_H
#define FIRE_DETECTION_H

#include <Arduino.h>

class FireDetection {
public:
  FireDetection(unsigned int updateIntervalMs = 1000);
  void update(float temperatureC, int airPPM, int lightLux);
  bool isFireDetected() const;
  float getFireScore() const;

private:
  static const int HISTORY_SIZE = 300;

  float tempHistory[HISTORY_SIZE];
  int airHistory[HISTORY_SIZE];
  int luxHistory[HISTORY_SIZE];
  unsigned long timeHistory[HISTORY_SIZE];

  int index = 0;
  bool filled = false;

  unsigned long lastUpdate = 0;
  unsigned int intervalMs;

  float fireScore = 0;

  float average(const float* arr, int size) const;
  float rateOfRise(const float* arr, const unsigned long* times, int size) const;
  int maxDrop(const int* arr, int size) const;

  void calculateScore();
};

#endif // FIRE_DETECTION_H