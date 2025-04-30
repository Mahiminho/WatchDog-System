#include "MqttManager.h"

MqttManager::MqttManager(
  const char* mqttServer,
  uint16_t mqttPort,
  const char* mqttClientId,
  const char* mqttTopic,
  WiFiClient& wifiClient
) :
  _mqttServer(mqttServer),
  _mqttPort(mqttPort),
  _mqttClientId(mqttClientId),
  _mqttTopic(mqttTopic),
  _wifiClient(wifiClient),
  _mqttClient(wifiClient) {
}

void MqttManager::begin() {
  Serial.println("Initializing MQTT...");
  _mqttClient.setServer(_mqttServer, _mqttPort); // set the MQTT server and port
  connect(); // attempt to connect to the broker
  Serial.println("MQTT initialized.\n");
}

void MqttManager::connect() {
  while (!_mqttClient.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (_mqttClient.connect(_mqttClientId)) { // attempt to connect with the client ID
      Serial.println("Connected.");
    }
    else {
      Serial.println("Failed. Retrying in 1 second.");
      delay(1000);
    }
  }
}

bool MqttManager::isConnected() {
  return _mqttClient.connected();
}

bool MqttManager::sendJson(const String& jsonPayload) {
  if (!_mqttClient.connected()) { // reconnect if disconnected
    Serial.println("MQTT disconnected. Reconnecting.");
    connect();
  }

  _mqttClient.loop(); // ensure background tasks are handled

  Serial.println("Publishing message to topic.");
  if (_mqttClient.publish(_mqttTopic, jsonPayload.c_str())) { // publish the JSON payload
    Serial.println("Success.");
    return true;
  }
  else {
    Serial.println("Failed.");
    return false;
  }
}