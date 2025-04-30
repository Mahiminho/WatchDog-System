#include "JsonSerializer.h"

JsonSerializer::JsonSerializer() {
  // constructor implementation (if necessary)
}

String JsonSerializer::toJson(
  float temperatureCelsius,
  float airQualityPPM,
  float lightIntensityLux,
  bool presenceDetected,
  bool motionDetected,
  float noiseLevelDB,
  bool vibrationDetected,
  float fireDetectionScore,
  bool fireDetected
) {
  StaticJsonDocument<256> doc;

  doc["temperature"] = temperatureCelsius;
  doc["airPPM"] = airQualityPPM;
  doc["lightLux"] = lightIntensityLux;
  doc["presence"] = presenceDetected;
  doc["motion"] = motionDetected;
  doc["noiseDB"] = noiseLevelDB;
  doc["vibration"] = vibrationDetected;
  doc["fireScore"] = fireDetectionScore;
  doc["fireDetected"] = fireDetected;

  String output;
  serializeJson(doc, output);
  return output;
}