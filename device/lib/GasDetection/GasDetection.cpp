#include "GasDetection.h"

GasDetection::GasDetection(unsigned int updateIntervalMs)
  : intervalMs(updateIntervalMs) {
  memset(ppmHistory, 0, sizeof(ppmHistory));
  memset(timeHistory, 0, sizeof(timeHistory));
  lastUpdate = millis();
}

void GasDetection::update(int airPPM) {
  unsigned long now = millis();
  if (now - lastUpdate < intervalMs) return;
  lastUpdate = now;

  float filteredPPM = applyKalmanFilter(static_cast<float>(airPPM));

  ppmHistory[index] = filteredPPM;
  timeHistory[index] = now;

  index = (index + 1) % HISTORY_SIZE;
  if (index == 0) filled = true;

  calculateScore();
}

float GasDetection::applyKalmanFilter(float measurement) {
  float kalmanGain = kalmanError / (kalmanError + measurementNoise);
  kalmanEstimate = kalmanEstimate + kalmanGain * (measurement - kalmanEstimate);
  kalmanError = (1 - kalmanGain) * kalmanError + fabs(kalmanEstimate - measurement) * processNoise;
  return kalmanEstimate;
}

float GasDetection::average(const float* arr, int size) const {
  float sum = 0;
  int count = filled ? HISTORY_SIZE : index;
  for (int i = 0; i < count; ++i) {
    sum += arr[i];
  }
  return (count > 0) ? sum / count : 0.0f;
}

float GasDetection::maxRise(const float* arr, int size) const {
  int count = filled ? HISTORY_SIZE : index;
  float maxDiff = 0.0f;
  for (int i = 1; i < count; ++i) {
    float diff = arr[i] - arr[i - 1];
    if (diff > maxDiff) maxDiff = diff;
  }
  return maxDiff;
}

void GasDetection::calculateScore() {
  int count = filled ? HISTORY_SIZE : index;
  if (count < 5) {
    dangerScore = 0.0f;
    return;
  }

  float avgPPM = average(ppmHistory, count);
  float rise = maxRise(ppmHistory, count);

  float ppmScore = constrain((avgPPM - 1000.0f) / (5000.0f - 1000.0f), 0.0f, 1.0f);
  float riseScore = constrain(rise / 100.0f, 0.0f, 1.0f);

  float totalScore = ppmScore * 0.7f + riseScore * 0.3f;

  float newDangerScore = constrain(totalScore * 100.0f, 0.0f, 100.0f);

  if (newDangerScore < dangerScore) {
    float decayRate = 0.5f;
    dangerScore = dangerScore - (dangerScore - newDangerScore) * decayRate;

    if (newDangerScore < 20.0f && dangerScore > 30.0f) {
      dangerScore -= 5.0f;
    }
  }
  else {
    dangerScore = newDangerScore;
  }

  dangerScore = constrain(dangerScore, 0.0f, 100.0f);
  dangerScore = round(dangerScore * 100.0f) / 100.0f;
}

bool GasDetection::isDangerous() const {
  return dangerScore >= 70.0f;
}

float GasDetection::getDangerScore() const {
  return dangerScore;
}