#include "Camera.h"

Camera::Camera(int rxPin, int txPin)
  : m_rxPin(rxPin), m_txPin(txPin), m_serial(Serial1) {
}

void Camera::begin() {
  Serial.println("Initializing camera UART...");
  m_serial.begin(CAMERA_UART_BAUD_RATE, SERIAL_8N1, m_rxPin, m_txPin);
  Serial.println("Camera UART initialized.\n");
}

void Camera::sendNetworkData(const String& ssid, const String& password) {
  while (m_serial.available()) m_serial.read();
  String data = ssid + " " + password + "\n";
  m_serial.print(data);
}

String Camera::receiveUrl(uint16_t timeoutMs) {
  unsigned long startTime = millis();
  String url = "";

  while (millis() - startTime < timeoutMs) {
    if (m_serial.available()) {
      String line = m_serial.readStringUntil('\n');
      line.trim();
      Serial.print("Received line: ");
      Serial.println(line);
      if (line.startsWith("http://") || line.startsWith("https://")) {
        url = line;
        break;
      }
    }
    delay(10);
  }
  return url;
}

// String Camera::sendNetworkDataAndGetUrl(const String& ssid, const String& password, uint16_t timeoutMs) {
//   while (m_serial.available()) m_serial.read();

//   String data = ssid + " " + password + "\n";
//   m_serial.print(data);

//   unsigned long startTime = millis();
//   String url = "";

//   while (millis() - startTime < timeoutMs) {
//     if (m_serial.available()) {
//       String line = m_serial.readStringUntil('\n');
//       line.trim();
//       Serial.print("Received line: ");
//       Serial.println(line);
//       if (line.startsWith("http://") || line.startsWith("https://")) {
//         url = line;
//         break;
//       }
//     }
//     delay(10);
//   }

//   return url;
// }