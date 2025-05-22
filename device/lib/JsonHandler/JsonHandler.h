#ifndef JSON_SERIALIZER_H
#define JSON_SERIALIZER_H

#include <ArduinoJson.h>

class JsonHandler {
public:
  JsonHandler();
  String toJson(
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
  );
};

#endif // JSON_SERIALIZER_H