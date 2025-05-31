#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <WiFiClient.h>
#include <PubSubClient.h>
#include <WiFi.h>

class MqttManager {
public:
  MqttManager(
    const char* mqttServer,
    uint16_t mqttPort,
    const char* mqttClientId,
    const char* mqttTopic,
    WiFiClient& wifiClient
  );
  void begin();
  bool sendJson(const String& jsonPayload);
  bool isConnected();
  bool sendRaw(const String& payload, const char* topic);

private:
  void connect();
  const char* m_mqttServer;
  uint16_t m_mqttPort;
  const char* m_mqttClientId;
  const char* m_mqttTopic;
  WiFiClient& m_wifiClient;
  PubSubClient m_mqttClient;
};

#endif // MQTT_MANAGER_H