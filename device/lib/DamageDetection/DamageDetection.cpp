#include "DamageDetection.h"

DamageDetection::DamageDetection(unsigned int updateIntervalMs)
  : intervalMs(updateIntervalMs) {
  memset(vibrationHistory, 0, sizeof(vibrationHistory));
  memset(presenceHistory, 0, sizeof(presenceHistory));
  memset(distanceHistory, 0, sizeof(distanceHistory));
  memset(timeHistory, 0, sizeof(timeHistory));
  lastUpdate = millis();
}

void DamageDetection::update(bool vibrationDetected, bool presenceDetected, float distanceM) {
  unsigned long now = millis();
  if (now - lastUpdate < intervalMs) return;
  lastUpdate = now;

  vibrationHistory[index] = vibrationDetected;
  presenceHistory[index] = presenceDetected;
  distanceHistory[index] = distanceM;
  timeHistory[index] = now;

  index = (index + 1) % HISTORY_SIZE;
  if (index == 0) filled = true;

  calculateScore();
}

bool DamageDetection::isDamageLikely() const {
  return damageScore >= 70.0f;
}

float DamageDetection::getDamageScore() const {
  return damageScore;
}

int DamageDetection::countTrue(const bool* arr, int size) const {
  int count = filled ? HISTORY_SIZE : index;
  int result = 0;
  for (int i = 0; i < count; ++i) {
    if (arr[i]) result++;
  }
  return result;
}

float DamageDetection::average(const float* arr, int size) const {
  int count = filled ? HISTORY_SIZE : index;
  float sum = 0;
  for (int i = 0; i < count; ++i) {
    sum += arr[i];
  }
  return (count > 0) ? sum / count : 0.0f;
}

void DamageDetection::calculateScore() {
  int count = filled ? HISTORY_SIZE : index;
  if (count < 5) {
    damageScore = 0.0f;
    return;
  }

  int vibCount = countTrue(vibrationHistory, count);
  int presCount = countTrue(presenceHistory, count);
  float avgDist = average(distanceHistory, count);

  float vibScore = constrain((float)vibCount / count, 0.0f, 1.0f); // vibration presence frequency
  float presScore = constrain((float)presCount / count, 0.0f, 1.0f); // presence detection frequency
  float distScore = constrain((1.0f - avgDist) / 1.0f, 0.0f, 1.0f); // closer than 1 meter is more suspicious

  float totalScore =
    vibScore * 0.4f +    // vibration is important
    presScore * 0.3f +   // presence also matters
    distScore * 0.3f;    // close distance is suspicious

  float newDamageScore = constrain(totalScore * 100.0f, 0.0f, 100.0f);

  if (newDamageScore < damageScore) {
    float decayRate = 0.5f;
    damageScore = damageScore - (damageScore - newDamageScore) * decayRate;

    if (newDamageScore < 20.0f && damageScore > 30.0f) {
      damageScore -= 5.0f;
    }
  }
  else {
    damageScore = newDamageScore;
  }

  damageScore = constrain(damageScore, 0.0f, 100.0f);
  damageScore = round(damageScore * 100.0f) / 100.0f;
}