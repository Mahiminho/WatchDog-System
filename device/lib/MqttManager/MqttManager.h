#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <WiFiClient.h>
#include <PubSubClient.h>

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

private:
  void connect();
  const char* _mqttServer;
  uint16_t _mqttPort;
  const char* _mqttClientId;
  const char* _mqttTopic;
  WiFiClient& _wifiClient;
  PubSubClient _mqttClient;
};

#endif // MQTT_MANAGER_H