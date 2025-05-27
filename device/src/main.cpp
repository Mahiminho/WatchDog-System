#include <Arduino.h>
#include <WiFi.h>
#include <SensorsData.h>
#include <FireDetection.h>
#include <GasDetection.h>
#include <HumanDetection.h>
#include <DamageDetection.h>
#include <JsonHandler.h>
#include <MqttManager.h>

#define THERMOMETER_PIN 4
#define AIR_QUALITY_PIN 34
#define LIGHT_ADDRESS 0x23
#define LIGHT_SDA_PIN 21
#define LIGHT_SCL_PIN 22
#define HUMAN_PRESENCE_RX_PIN 16
#define HUMAN_PRESENCE_TX_PIN 17
#define MOTION_DETECTION_PIN 26
#define AUDIO_PIN 32
#define VIBRATION_DETECTION_PIN 12

#define DELAY_MS 1000

// TODO:
// !) FIX THE AUDIO CLASS
// !!) Google how potentiometers works on MQ-135 and SW-420 to set the best sensitivity. Probably set better sensitivity for SR501
// 0) Reconfig LD2420 to more channels for bigger diapason of detection
// 1) Add functional, for getting data on second UART: just get URL and HTTP request to UI
// 2) Little refactoring of sensor classes (comments at least and structure)
// 3) MAKE MORE REAL CLASSES (FireDetection, GasDetection, HumanDetection, DamageDetection) (or the value is real?)
// 4) Class for work with JSON: parse JSON with instructions from server !!!
// 5) Class for work with MQTT: receive JSON from server !!!
// 6) Execution classes: lock (unlock auto if fire detected, lock if damage, lock/unlock by command), alarm (speaker + LED, auto if fire detected or damage)
// 7) Fill README files. Main file with instruction how to build, which sensors use, etc.
// 8) Tests ???

// MQTT broker settings
const char* ssid = "PIXEL";
const char* password = "20040517";
const char* mqttServer = "192.168.31.108";
const uint16_t mqttPort = 1883;
const char* mqttClientId = "esp32-client-01";
const char* mqttTopic = "device/sensors/data";

SensorsData sensors(
  THERMOMETER_PIN,
  AIR_QUALITY_PIN,
  LIGHT_ADDRESS,
  LIGHT_SDA_PIN,
  LIGHT_SCL_PIN,
  HUMAN_PRESENCE_RX_PIN,
  HUMAN_PRESENCE_TX_PIN,
  MOTION_DETECTION_PIN,
  AUDIO_PIN,
  VIBRATION_DETECTION_PIN
);
FireDetection fireDetection(DELAY_MS);
GasDetection gasDetection(DELAY_MS);
HumanDetection humanDetection(DELAY_MS);
DamageDetection damageDetection(DELAY_MS);
JsonHandler jsonHandler;
WiFiClient wifiClient;
MqttManager mqtt(mqttServer, mqttPort, mqttClientId, mqttTopic, wifiClient);

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing sensors...\n");
  sensors.initSensors();
  Serial.println("Sensors initialized.\n");

  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 60) {
    delay(DELAY_MS);
    attempts++;
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection failed! Restarting ESP32...\n");
    ESP.restart();
  }
  Serial.println("WiFi connected!\n");
  Serial.printf("IP address: %s\n\n", WiFi.localIP().toString().c_str());
  delay(DELAY_MS);

  mqtt.begin();
}

void loop() {
  // collect data from sensors
  sensors.collectData();
  SensorReadings readings = sensors.getData();

  // update detection classes
  fireDetection.update(readings.temperatureCelsius, readings.airQualityPPM, readings.lightIntensityLux);
  float fireDetectionScore = fireDetection.getFireScore();
  bool fireDetected = fireDetection.isFireDetected();

  gasDetection.update(readings.airQualityPPM);
  float gasDetectionScore = gasDetection.getDangerScore();
  bool gasDetected = gasDetection.isDangerous();

  humanDetection.update(
    readings.motionDetected,
    readings.presenceDetected,
    readings.presenceDistance,
    readings.noiseLevelDB
  );
  bool presenceDetected = humanDetection.isHumanDetected();
  float presenceProbability = humanDetection.getPresenceProbability();

  damageDetection.update(
    readings.vibrationDetected,
    readings.presenceDetected,
    readings.presenceDistance
  );
  bool damageDetected = damageDetection.isDamageLikely();
  float damageDetectionScore = damageDetection.getDamageScore();

  // serialize data to JSON
  String jsonOutput = jsonHandler.toJson(
    readings.temperatureCelsius,
    readings.airQualityPPM,
    readings.lightIntensityLux,
    readings.presenceDetected,
    readings.presenceDistance,
    readings.motionDetected,
    readings.noiseLevelDB,
    readings.vibrationDetected,
    fireDetectionScore,
    fireDetected,
    gasDetectionScore,
    gasDetected,
    presenceProbability,
    presenceDetected,
    damageDetectionScore,
    damageDetected
  );

  // send JSON to MQTT broker
  bool mqttSendingStatus = mqtt.sendJson(jsonOutput);

  Serial.println("Sensor readings:");
  Serial.println("Temp: " + String(readings.temperatureCelsius) + " °C");
  Serial.println("Air PPM: " + String(readings.airQualityPPM) + " PPM");
  Serial.println("Light: " + String(readings.lightIntensityLux) + " Lux");
  Serial.println("Presence: " + String(readings.presenceDetected ? "Yes" : "No"));
  Serial.println("Presence distance: " + String(readings.presenceDistance) + " m");
  Serial.println("Motion: " + String(readings.motionDetected ? "Yes" : "No"));
  Serial.println("Noise: " + String(readings.noiseLevelDB) + " dB");
  Serial.println("Vibration: " + String(readings.vibrationDetected ? "Yes" : "No"));
  Serial.println();

  Serial.println("Detection results:");
  Serial.println("Fire detection score: " + String(fireDetectionScore) + " %");
  Serial.println("Fire detected: " + String(fireDetected ? "Yes" : "No"));
  Serial.println("Gas detection score: " + String(gasDetectionScore) + " %");
  Serial.println("Gas detected: " + String(gasDetected ? "Yes" : "No"));
  Serial.println("Human detection probability: " + String(presenceProbability) + " %");
  Serial.println("Human detected: " + String(presenceDetected ? "Yes" : "No"));
  Serial.println("Damage detection score: " + String(damageDetectionScore) + " %");
  Serial.println("Damage detected: " + String(damageDetected ? "Yes" : "No"));
  Serial.println();

  Serial.println("Serialized JSON:");
  Serial.println(jsonOutput);
  Serial.println();

  Serial.println("MQTT sending status: " + String(mqttSendingStatus ? "Success" : "Failed"));
  Serial.println();

  delay(DELAY_MS);
}