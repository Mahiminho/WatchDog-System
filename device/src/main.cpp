#include <Arduino.h>
#include <SensorsData.h>

#define THERMOMETER_PIN 4
#define AIR_QUALITY_PIN 34
#define LIGHT_ADDRESS 0x23
#define LIGHT_SDA_PIN 21
#define LIGHT_SCL_PIN 22
#define HUMAN_PRESENCE_PIN 27
#define MOTION_DETECTION_PIN 26
#define AUDIO_PIN 32
#define VIBRATION_DETECTION_PIN 12

// TODO:
// 1) Class for fire detection with sensor fusion alghorithms
// 2) Class for gas type (?) detection with sensor fusion alghorithms
// 3) Class for people detection
// 4) Class for damage detection
// 5) Class for work with JSON: create JSON with sensors data + parse JSON with instructions from server
// 6) Class for work with MQTT: send JSON to server + receive JSON from server
// 7) Execution class (?): lock, alarm (speaker + LED)
// 8) Fill README files. Main file with instruction how to build, which sensors use, etc.
// 9) Tests ???

SensorsData sensors;

void setup() {
  Serial.begin(9600);
  sensors.initSensors();
}

void loop() {
  sensors.collectData();
  SensorReadings readings = sensors.getData();

  Serial.println("Temp: " + String(readings.temperatureCelsius) + " °C");
  Serial.println("Air PPM: " + String(readings.airQualityPPM) + " PPM");
  Serial.println("Light: " + String(readings.lightIntensityLux) + " Lux");
  Serial.println("Presence: " + String(readings.presenceDetected ? "Yes" : "No"));
  Serial.println("Motion: " + String(readings.motionDetected ? "Yes" : "No"));
  Serial.println("Noise: " + String(readings.noiseLevelDB) + " dB");
  Serial.println("Vibration: " + String(readings.vibrationDetected ? "Yes" : "No"));
  Serial.println();

  delay(1000);
}