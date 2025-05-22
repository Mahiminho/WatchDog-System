#ifndef HUMAN_DETECTION_H
#define HUMAN_DETECTION_H

#include <Arduino.h>

class HumanDetection {
public:
  // інтервал між оновленнями (мс)
  HumanDetection(unsigned int updateIntervalMs = 1000);

  // викликається кожного інтервалу з поточними даними
  // motion – чи спрацював SR501
  // presence – чи спрацював LD2420
  // distanceM – відстань до об'єкта (м)
  // noiseDb – рівень шуму (дБ)
  void update(bool motion, bool presence, float distanceM, float noiseDb);

  // чи є людина (поріг за ймовірністю 50%)
  bool isHumanDetected() const;

  // ймовірність присутності людини [0–100]
  float getPresenceProbability() const;

private:
  static const int HISTORY_SIZE = 300;

  bool motionHistory[HISTORY_SIZE];
  bool presenceHistory[HISTORY_SIZE];
  float distanceHistory[HISTORY_SIZE];
  float noiseHistory[HISTORY_SIZE];
  unsigned long timeHistory[HISTORY_SIZE];

  int index = 0;
  bool filled = false;

  unsigned long lastUpdate = 0;
  unsigned int intervalMs;

  float presenceProb = 0.0f;

  // допоміжні методи
  float avgDistance(int count) const;
  float avgNoise(int count) const;
  int countTrue(const bool* arr, int count) const;

  void calculateProbability();
};

#endif // HUMAN_DETECTION_H