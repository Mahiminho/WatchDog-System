#include "JsonHandler.h"

JsonHandler::JsonHandler() {}

String JsonHandler::toJson(
  float temperatureCelsius,
  float airQualityPPM,
  float lightIntensityLux,
  bool presenceDetected,
  float presenceDistance,
  bool motionDetected,
  float noiseLevelDB,
  bool vibrationDetected,
  float fireDetectionScore,
  bool fireDetected,
  float gasDetectionScore,
  bool gasDetected,
  float humanDetectionScore,
  bool humanDetected,
  float damageDetectionScore,
  bool damageDetected
) {
  JsonDocument doc;

  doc["temperature"] = temperatureCelsius;
  doc["airPPM"] = airQualityPPM;
  doc["lightLux"] = lightIntensityLux;
  doc["presence"] = presenceDetected;
  doc["presenceDistance"] = presenceDistance;
  doc["motion"] = motionDetected;
  doc["noiseDB"] = noiseLevelDB;
  doc["vibration"] = vibrationDetected;
  doc["fireScore"] = fireDetectionScore;
  doc["fireDetected"] = fireDetected;
  doc["gasScore"] = gasDetectionScore;
  doc["gasDetected"] = gasDetected;
  doc["humanScore"] = humanDetectionScore;
  doc["humanDetected"] = humanDetected;
  doc["damageScore"] = damageDetectionScore;
  doc["damageDetected"] = damageDetected;

  String output;
  serializeJson(doc, output);
  return output;
}