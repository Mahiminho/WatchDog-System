#include <Arduino.h>
#include <SensorsData.h>
#include <FireDetection.h>
#include <JsonSerializer.h>
#include <MqttManager.h>
#include <WiFi.h>

#define THERMOMETER_PIN 4
#define AIR_QUALITY_PIN 34
#define LIGHT_ADDRESS 0x23
#define LIGHT_SDA_PIN 21
#define LIGHT_SCL_PIN 22
#define HUMAN_PRESENCE_PIN 27
#define MOTION_DETECTION_PIN 26
#define AUDIO_PIN 32
#define VIBRATION_DETECTION_PIN 12

#define DELAY_MS 1000

// TODO:
// 2) Class for gas type (?) detection with sensor fusion alghorithms     PAUSED
// 3) Class for people detection     PAUSED
// 4) Class for damage detection     PAUSED
// 5) Class for work with JSON: parse JSON with instructions from server !!!
// 6) Class for work with MQTT: receive JSON from server !!!
// 7) Execution classes (?): lock (unlock auto if fire detected, lock/unlock by command), alarm (speaker + LED, auto if fire detected)
// 8) Fill README files. Main file with instruction how to build, which sensors use, etc.
// 9) Tests ???

const char* ssid = "PIXEL";
const char* password = "20040517";
const char* mqttServer = "192.168.31.108";
const uint16_t mqttPort = 1883;
const char* mqttClientId = "esp32-client";
const char* mqttTopic = "sensors/json";

SensorsData sensors(
  THERMOMETER_PIN,
  AIR_QUALITY_PIN,
  LIGHT_ADDRESS,
  LIGHT_SDA_PIN,
  LIGHT_SCL_PIN,
  HUMAN_PRESENCE_PIN,
  MOTION_DETECTION_PIN,
  AUDIO_PIN,
  VIBRATION_DETECTION_PIN
);
FireDetection fireDetection(DELAY_MS);
JsonSerializer jsonSerializer;
WiFiClient wifiClient;
MqttManager mqtt(mqttServer, mqttPort, mqttClientId, mqttTopic, wifiClient);

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing sensors...\n");
  sensors.initSensors();
  Serial.println("Sensors initialized.\n");

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
    delay(DELAY_MS);
  Serial.println("Connected to WiFi.\n\n\n");
  mqtt.begin();
}

void loop() {
  sensors.collectData();
  SensorReadings readings = sensors.getData();

  fireDetection.update(readings.temperatureCelsius, readings.airQualityPPM, readings.lightIntensityLux);
  float fireDetectionScore = fireDetection.getFireScore();
  bool fireDetected = fireDetection.isFireDetected();

  String jsonOutput = jsonSerializer.toJson(
    readings.temperatureCelsius,
    readings.airQualityPPM,
    readings.lightIntensityLux,
    readings.presenceDetected,
    readings.motionDetected,
    readings.noiseLevelDB,
    readings.vibrationDetected,
    fireDetectionScore,
    fireDetected
  );

  bool mqttSendingStatus = mqtt.sendJson(jsonOutput);

  Serial.println("Sensor readings:");
  Serial.println("Temp: " + String(readings.temperatureCelsius) + " °C");
  Serial.println("Air PPM: " + String(readings.airQualityPPM) + " PPM");
  Serial.println("Light: " + String(readings.lightIntensityLux) + " Lux");
  Serial.println("Presence: " + String(readings.presenceDetected ? "Yes" : "No"));
  Serial.println("Motion: " + String(readings.motionDetected ? "Yes" : "No"));
  Serial.println("Noise: " + String(readings.noiseLevelDB) + " dB");
  Serial.println("Vibration: " + String(readings.vibrationDetected ? "Yes" : "No"));
  Serial.println();

  Serial.println("Fire detection score: " + String(fireDetectionScore) + " %");
  Serial.println("Fire detected: " + String(fireDetected ? "Yes" : "No"));
  Serial.println();

  Serial.println("Serialized JSON:");
  Serial.println(jsonOutput);
  Serial.println();

  Serial.println("MQTT sending status: " + String(mqttSendingStatus ? "Success" : "Failed"));
  Serial.println();

  delay(DELAY_MS);
}