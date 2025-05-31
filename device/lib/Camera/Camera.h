#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>

#define CAMERA_UART_BAUD_RATE 115200

class Camera {
public:
  Camera(int rxPin = 25, int txPin = 13);
  void begin();
  // String sendNetworkDataAndGetUrl(const String& ssid, const String& password, uint16_t timeoutMs = 5000);
  void sendNetworkData(const String& ssid, const String& password);
  String receiveUrl(uint16_t timeoutMs = 1000);

private:
  int m_rxPin;
  int m_txPin;
  HardwareSerial& m_serial;
};

#endif // CAMERA_H