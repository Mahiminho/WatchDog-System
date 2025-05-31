#include "HumanDetection.h"

HumanDetection::HumanDetection(unsigned int updateIntervalMs)
  : intervalMs(updateIntervalMs) {
  memset(motionHistory, 0, sizeof(motionHistory));
  memset(presenceHistory, 0, sizeof(presenceHistory));
  memset(distanceHistory, 0, sizeof(distanceHistory));
  memset(noiseHistory, 0, sizeof(noiseHistory));
  memset(timeHistory, 0, sizeof(timeHistory));
  lastUpdate = millis();
}

void HumanDetection::update(bool motion, bool presence, float distanceM, float noiseDb) {
  unsigned long now = millis();
  if (now - lastUpdate < intervalMs) return;
  lastUpdate = now;

  motionHistory[index] = motion;
  presenceHistory[index] = presence;
  distanceHistory[index] = distanceM;
  noiseHistory[index] = noiseDb;
  timeHistory[index] = now;

  index = (index + 1) % HISTORY_SIZE;
  if (index == 0) filled = true;

  calculateProbability();
}

bool HumanDetection::isHumanDetected() const {
  return presenceProb >= 70.0f;
}

float HumanDetection::getPresenceProbability() const {
  return presenceProb;
}

float HumanDetection::avgDistance(int count) const {
  if (count == 0) return 0.0f;
  float sum = 0;
  for (int i = 0; i < count; ++i) sum += distanceHistory[i];
  return sum / count;
}

float HumanDetection::avgNoise(int count) const {
  if (count == 0) return 0.0f;
  float sum = 0;
  for (int i = 0; i < count; ++i) sum += noiseHistory[i];
  return sum / count;
}

int HumanDetection::countTrue(const bool* arr, int count) const {
  int c = 0;
  for (int i = 0; i < count; ++i) if (arr[i]) ++c;
  return c;
}

void HumanDetection::calculateProbability() {
  int count = filled ? HISTORY_SIZE : index;
  if (count < 5) {
    presenceProb = 0.0f;
    return;
  }

  // якщо є рух і присутність бодай раз – максимальна впевненість
  if (countTrue(motionHistory, count) > 0 && countTrue(presenceHistory, count) > 0) {
    presenceProb = 100.0f;
    return;
  }

  // базова ймовірність на основі середньої відстані (чим ближче – тим вище)
  float d = avgDistance(count);
  float distScore = constrain((5.0f - d) / 5.0f, 0.0f, 1.0f);

  // додатковий бонус від рівня шуму (високий шум – більше шансів)
  float n = avgNoise(count);
  float noiseScore = constrain(n / 80.0f, 0.0f, 1.0f); // 0–80 дБ

  // якщо датчик присутності спрацьовував хоча б раз – додаємо фіксований бонус
  float presBonus = (countTrue(presenceHistory, count) > 0) ? 0.2f : 0.0f;

  // зважена сума
  float total = distScore * 0.5f + noiseScore * 0.3f + presBonus * 0.2f;
  float newProb = constrain(total * 100.0f, 0.0f, 100.0f);

  // плавне згасання показника, якщо ймовірність падає
  if (newProb < presenceProb) {
    presenceProb -= (presenceProb - newProb) * 0.5f;
  }
  else {
    presenceProb = newProb;
  }

  presenceProb = constrain(presenceProb, 0.0f, 100.0f);
  presenceProb = round(presenceProb * 100.0f) / 100.0f;
}