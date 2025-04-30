#include "FireDetection.h"

FireDetection::FireDetection(unsigned int updateIntervalMs)
  : intervalMs(updateIntervalMs) {
  memset(tempHistory, 0, sizeof(tempHistory)); // initialize history arrays to zero
  memset(airHistory, 0, sizeof(airHistory));
  memset(luxHistory, 0, sizeof(luxHistory));
  memset(timeHistory, 0, sizeof(timeHistory));
  lastUpdate = millis(); // record the last update time
}

void FireDetection::update(float temperatureC, int airPPM, int lightLux) { // update the sensor data and calculate fire detection score
  unsigned long now = millis();
  if (now - lastUpdate < intervalMs) return; // ensure updates occur only after the specified interval
  lastUpdate = now;

  tempHistory[index] = temperatureC; // store the latest sensor data in history arrays
  airHistory[index] = airPPM;
  luxHistory[index] = lightLux;
  timeHistory[index] = now;

  index = (index + 1) % HISTORY_SIZE; // update the index and mark history as filled if necessary
  if (index == 0) filled = true;

  calculateScore(); // recalculate the fire detection score
}

bool FireDetection::isFireDetected() const {
  return fireScore >= 70.0f; // fire is detected if the score is 70 or higher
}

float FireDetection::getFireScore() const {
  return fireScore;
}

float FireDetection::average(const float* arr, int size) const {
  float sum = 0;
  int count = filled ? HISTORY_SIZE : index; // use full history if filled
  for (int i = 0; i < count; ++i) {
    sum += arr[i];
  }
  return (count > 0) ? sum / count : 0.0f; // calculate the average of the array
}

float FireDetection::rateOfRise(const float* arr, const unsigned long* times, int size) const { // calculate the rate of rise in temperature
  int count = filled ? HISTORY_SIZE : index;
  if (count < 2) return 0.0f; // not enough data points

  float maxRate = 0.0f;
  for (int i = 1; i < count; ++i) {
    float deltaT = arr[i] - arr[i - 1];
    float deltaTime = (times[i] - times[i - 1]) / 1000.0f; // convert to seconds
    if (deltaTime > 0) {
      float rate = deltaT / deltaTime;
      if (rate > maxRate) maxRate = rate; // track the maximum rate
    }
  }
  return maxRate;
}

int FireDetection::maxDrop(const int* arr, int size) const { // calculate the maximum drop in air quality
  int count = filled ? HISTORY_SIZE : index;
  int maxDiff = 0;
  for (int i = 1; i < count; ++i) {
    int diff = arr[i - 1] - arr[i];
    if (diff > maxDiff) maxDiff = diff; // track the maximum drop
  }
  return maxDiff;
}

void FireDetection::calculateScore() {
  int count = filled ? HISTORY_SIZE : index;
  if (count < 5) { // not enough data points to calculate a score
    fireScore = 0.0f;
    return;
  }

  float avgTemp = average(tempHistory, count);
  float tempRise = rateOfRise(tempHistory, timeHistory, count);
  int airDrop = maxDrop(airHistory, count);
  float avgLux = average((float*)luxHistory, count);

  float tempScore = constrain((avgTemp - 35.0f) / (80.0f - 35.0f), 0.0f, 1.0f); // 35°C to 80°C
  float riseScore = constrain(tempRise / 2.0f, 0.0f, 1.0f);                     // 0°C/s to 2°C/s
  float airDropScore = constrain((float)airDrop / 500.0f, 0.0f, 1.0f);          // 0 to 500 PPM drop
  float luxScore = constrain((20.0f - avgLux) / 20.0f, 0.0f, 1.0f);             // 0 lux — dark, up to 20 lux

  float totalScore =
    tempScore * 0.4f +      // temperature has the highest weight
    riseScore * 0.3f +      // sharp heating is an important indicator
    airDropScore * 0.2f +   // drop in air quality — smoke
    luxScore * 0.1f;        // reduced lighting may indicate smoke

  float newFireScore = constrain(totalScore * 100.0f, 0.0f, 100.0f);

  if (newFireScore < fireScore) {
    float decayRate = 0.5f; // decay rate for the fire score
    fireScore = fireScore - (fireScore - newFireScore) * decayRate;

    if (newFireScore < 20.0f && fireScore > 30.0f) {
      fireScore -= 5.0f; // apply a penalty if the score drops significantly
    }
  }
  else {
    fireScore = newFireScore; // update the fire score if it increases
  }

  fireScore = constrain(fireScore, 0.0f, 100.0f); // ensure the fire score is within valid bounds
  fireScore = round(fireScore * 100.0f) / 100.0f;
}