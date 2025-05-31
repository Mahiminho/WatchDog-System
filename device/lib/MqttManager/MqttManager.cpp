#include "MqttManager.h"

MqttManager::MqttManager(
  const char* mqttServer,
  uint16_t mqttPort,
  const char* mqttClientId,
  const char* mqttTopic,
  WiFiClient& wifiClient
) :
  m_mqttServer(mqttServer),
  m_mqttPort(mqttPort),
  m_mqttClientId(mqttClientId),
  m_mqttTopic(mqttTopic),
  m_wifiClient(wifiClient),
  m_mqttClient(wifiClient) {
}

void MqttManager::begin() {
  Serial.println("Initializing MQTT...");
  m_mqttClient.setServer(m_mqttServer, m_mqttPort); // set the MQTT server and port
  m_mqttClient.setBufferSize(512);
  connect(); // attempt to connect to the broker
  Serial.println("MQTT initialized.\n");
}

void MqttManager::connect() {
  while (!m_mqttClient.connected()) {
    Serial.println("Attempting MQTT connection...");
    Serial.printf("MQTT Server: %s\n", m_mqttServer);
    Serial.printf("MQTT Port: %d\n", m_mqttPort);
    Serial.printf("Client ID: %s\n", m_mqttClientId);
    Serial.printf("WiFi Status: %s\n", WiFi.status() == WL_CONNECTED ? "Connected" : "Something wrong with WiFi");
    Serial.printf("Local IP: %s\n", WiFi.localIP().toString().c_str());

    if (!m_wifiClient.connect(m_mqttServer, m_mqttPort)) { // check if the client can connect to the broker
      Serial.println("Failed to connect to MQTT broker directly. Something wrong with broker!");
      delay(1000); // wait for 1 second before retrying
      continue;
    }

    if (m_mqttClient.connect(m_mqttClientId)) { // attempt to connect with the client ID
      Serial.println("Connected to MQTT broker.\n\n\n");
      return;
    }
    else {
      int state = m_mqttClient.state();
      Serial.printf("Failed with error code: %d\n", state);
      if (state == -4) {
        Serial.println("Error: MQTT_CONNECTION_TIMEOUT - the server didn't respond within the keepalive time.");
      }
      else if (state == -3) {
        Serial.println("Error: MQTT_CONNECTION_LOST - the network connection was broken.");
      }
      else if (state == -2) {
        Serial.println("Error: MQTT_CONNECT_FAILED - the network connection failed.");
      }
      else if (state == -1) {
        Serial.println("Error: MQTT_DISCONNECTED - the client is disconnected.");
      }
      else if (state == 0) {
        Serial.println("Error: MQTT_CONNECTED - the client is connected.");
      }
      else if (state == 1) {
        Serial.println("Error: MQTT_CONNECT_BAD_PROTOCOL - incorrect protocol version.");
      }
      else if (state == 2) {
        Serial.println("Error: MQTT_CONNECT_BAD_CLIENT_ID - client ID rejected.");
      }
      else if (state == 3) {
        Serial.println("Error: MQTT_CONNECT_UNAVAILABLE - server unavailable.");
      }
      else if (state == 4) {
        Serial.println("Error: MQTT_CONNECT_BAD_CREDENTIALS - bad username/password.");
      }
      else if (state == 5) {
        Serial.println("Error: MQTT_CONNECT_UNAUTHORIZED - unauthorized.");
      }
      else {
        Serial.println("Error: Unknown error code.");
      }

      Serial.println("Failed. Retrying in 1 second.");
      delay(1000);
    }
  }
}

bool MqttManager::isConnected() {
  return m_mqttClient.connected();
}

bool MqttManager::sendJson(const String& jsonPayload) {
  if (!m_mqttClient.connected()) { // reconnect if disconnected
    Serial.println("MQTT disconnected. Reconnecting!");
    connect();
  }

  m_mqttClient.loop(); // ensure background tasks are handled

  Serial.println("Publishing message to topic.");
  if (m_mqttClient.publish(m_mqttTopic, jsonPayload.c_str())) { // publish the JSON payload
    Serial.println("Success.");
    return true;
  }
  else {
    Serial.println("Failed.");
    return false;
  }
}

bool MqttManager::sendRaw(const String& payload, const char* topic) {
  if (!m_mqttClient.connected()) return false;
  return m_mqttClient.publish(topic, payload.c_str(), true);
}