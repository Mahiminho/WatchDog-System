#ifndef JSON_SERIALIZER_H
#define JSON_SERIALIZER_H

#include <ArduinoJson.h>

class JsonSerializer {
public:
  JsonSerializer();
  String toJson(
    float temperatureCelsius,
    float airQualityPPM,
    float lightIntensityLux,
    bool presenceDetected,
    bool motionDetected,
    float noiseLevelDB,
    bool vibrationDetected,
    float fireDetectionScore,
    bool fireDetected
  );
};

#endif // JSON_SERIALIZER_H