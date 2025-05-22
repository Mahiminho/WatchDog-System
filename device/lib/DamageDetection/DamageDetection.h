#ifndef DAMAGE_DETECTION_H
#define DAMAGE_DETECTION_H

#include <Arduino.h>

class DamageDetection {
public:
  DamageDetection(unsigned int updateIntervalMs = 1000);
  void update(bool vibrationDetected, bool presenceDetected, float distanceM);
  bool isDamageLikely() const;
  float getDamageScore() const;

private:
  static const int HISTORY_SIZE = 300;

  bool vibrationHistory[HISTORY_SIZE];
  bool presenceHistory[HISTORY_SIZE];
  float distanceHistory[HISTORY_SIZE];
  unsigned long timeHistory[HISTORY_SIZE];

  int index = 0;
  bool filled = false;

  unsigned long lastUpdate = 0;
  unsigned int intervalMs;

  float damageScore = 0;

  int countTrue(const bool* arr, int size) const;
  float average(const float* arr, int size) const;

  void calculateScore();
};

#endif // DAMAGE_DETECTION_H