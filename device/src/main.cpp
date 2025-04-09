#include <Arduino.h>
#include <Thermometer.h>
#include <AirQuality.h>
#include <Light.h>
#include <HumanPresence.h>
#include <MotionDetection.h>
#include <Audio.h>
#include <VibrationDetection.h>

#define THERMOMETER_PIN 4
#define AIR_QUALITY_PIN 34
#define LIGHT_ADDRESS 0x23
#define LIGHT_SDA_PIN 21
#define LIGHT_SCL_PIN 22
#define HUMAN_PRESENCE_PIN 27
#define MOTION_DETECTION_PIN 26
#define AUDIO_PIN 32
#define VIBRATION_DETECTION_PIN 12

Thermometer thermometer(THERMOMETER_PIN);
AirQuality airQuality(AIR_QUALITY_PIN);
Light light(LIGHT_ADDRESS, LIGHT_SDA_PIN, LIGHT_SCL_PIN);
HumanPresence humanPresence(HUMAN_PRESENCE_PIN);
MoveDetection motionDetection(MOTION_DETECTION_PIN);
Audio audio(AUDIO_PIN);
VibrationDetection vibrationDetection(VIBRATION_DETECTION_PIN);

void setup() {
  Serial.begin(9600);
  thermometer.begin();
  airQuality.begin();
  light.begin();
  humanPresence.begin();
  motionDetection.begin();
  audio.begin();
  vibrationDetection.begin();
}

void loop() {
  float temperatureCelsius = thermometer.getTemperature();
  int airQualityPPM = airQuality.readPPM();
  float lightIntensityLux = light.readLux();
  bool presenceDetected = humanPresence.isPresent();
  bool motionDetected = motionDetection.isMotionDetected();
  float noiseLevelDB = audio.getNoiseLevel();
  bool vibrationDetected = vibrationDetection.isVibrationDetected();

  delay(1000);
}