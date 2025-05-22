#ifndef HUMANPRESENCE_H
#define HUMANPRESENCE_H

#include <Arduino.h>

#define UART_BAUD_RATE 115200

class HumanPresence {
public:
  HumanPresence(int rxPin = 16, int txPin = 17);
  void begin();
  bool isPresent();
  float distance();

private:
  int m_rxPin;
  int m_txPin;
  HardwareSerial& m_serial;
  bool m_present = false;
  float m_lastDistance = -1.0f;

  void parseSerial();
};

#endif // HUMANPRESENCE_H