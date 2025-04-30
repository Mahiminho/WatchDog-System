#ifndef SENSORSDATA_H
#define SENSORSDATA_H

#include <Arduino.h>
#include <Thermometer.h>
#include <AirQuality.h>
#include <Light.h>
#include <HumanPresence.h>
#include <MotionDetection.h>
#include <Audio.h>
#include <VibrationDetection.h>

typedef struct {
  float temperatureCelsius;
  int airQualityPPM;
  int lightIntensityLux;
  bool presenceDetected;
  bool motionDetected;
  float noiseLevelDB;
  bool vibrationDetected;
} SensorReadings;

class SensorsData :
  public Thermometer,
  public AirQuality,
  public Light,
  public HumanPresence,
  public MoveDetection,
  public Audio,
  public VibrationDetection {
public:
  SensorsData(uint8_t thermometer_pin = 4,
              uint8_t air_quality_pin = 34,
              uint8_t light_address = 0x23,
              uint8_t light_sda_pin = 21,
              uint8_t light_scl_pin = 22,
              uint8_t human_presence_pin = 27,
              uint8_t motion_detection_pin = 26,
              uint8_t audio_pin = 32,
              uint8_t vibration_detection_pin = 12); // default pins
  void initSensors();
  void collectData();
  SensorReadings getData() const;

private:
  SensorReadings data;
};

#endif // SENSORSDATA_H