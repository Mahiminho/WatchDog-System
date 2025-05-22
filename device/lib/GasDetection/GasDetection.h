#ifndef GAS_DETECTION_H
#define GAS_DETECTION_H

#include <Arduino.h>

class GasDetection {
public:
  GasDetection(unsigned int updateIntervalMs = 1000);
  void update(int airPPM);
  bool isDangerous() const;
  float getDangerScore() const;

private:
  static const int HISTORY_SIZE = 300;

  float ppmHistory[HISTORY_SIZE];
  unsigned long timeHistory[HISTORY_SIZE];

  int index = 0;
  bool filled = false;

  unsigned long lastUpdate = 0;
  unsigned int intervalMs;

  float dangerScore = 0;

  float kalmanEstimate = 0.0f;
  float kalmanError = 1.0f;
  const float processNoise = 1.0f;
  const float measurementNoise = 10.0f;

  float applyKalmanFilter(float measurement);
  float average(const float* arr, int size) const;
  float maxRise(const float* arr, int size) const;

  void calculateScore();
};

#endif // GAS_DETECTION_H