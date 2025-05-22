// #include "HumanPresence.h"

// HumanPresence::HumanPresence(uint8_t sensorPresencePin) {
//   m_sensorPresencePin = sensorPresencePin;
// }

// void HumanPresence::begin() {
//   Serial.println("Initializing human presence sensor...");
//   pinMode(m_sensorPresencePin, INPUT);
//   Serial.println("Human presence sensor initialized.\n");
// }

// bool HumanPresence::isPresent() {
//   Serial.println("Checking human presence...");
//   bool presenceDetected = digitalRead(m_sensorPresencePin) == HIGH;
//   Serial.println("Human presence checked.");
//   return presenceDetected;
// }

#include "HumanPresence.h"

HumanPresence::HumanPresence(int rxPin, int txPin)
  : m_rxPin(rxPin), m_txPin(txPin), m_serial(Serial2) {
}

void HumanPresence::begin() {
  Serial.println("Initializing human presence sensor...");
  m_serial.begin(UART_BAUD_RATE, SERIAL_8N1, m_rxPin, m_txPin);
  Serial.println("Human presence sensor initialized.\n");
}

bool HumanPresence::isPresent() {
  parseSerial();
  return m_present;
}

float HumanPresence::distance() {
  parseSerial();
  if (!m_present) return -1.0f;
  return roundf(m_lastDistance * 100.0f) / 100.0f;
}

void HumanPresence::parseSerial() {
  static String line = "";

  while (m_serial.available()) {
    char ch = m_serial.read();

    if (ch == '\n' || ch == '\r') {
      line.trim();

      if (line.equalsIgnoreCase("ON")) {
        m_present = true;
      }
      else if (line.equalsIgnoreCase("OFF")) {
        m_present = false;
      }
      else if (line.startsWith("Range")) {
        int value = line.substring(6).toInt();
        m_lastDistance = value / 100;
      }

      line = "";
    }
    else {
      line += ch;
      if (line.length() > 32) {
        line = "";
      }
    }
  }
}





// float HumanPresence::distance() {
//   parseSerial();
//   if (!m_present) return -1.0f;
//   return roundf(m_lastDistance * 100.0f) / 100.0f;
// }

// void HumanPresence::parseSerial() {
//   static String line = "";

//   while (m_serial.available()) {
//     char ch = m_serial.read();

//     if (ch == '\n' || ch == '\r') {
//       line.trim();

//       if (line.equalsIgnoreCase("ON")) {
//         m_present = true;
//       }
//       else if (line.equalsIgnoreCase("OFF")) {
//         m_present = false;
//       }
//       else if (line.startsWith("Range")) {
//         int value = line.substring(6).toInt();
//         m_lastDistance = value / 100;
//       }

//       line = "";
//     }
//     else {
//       line += ch;
//       if (line.length() > 32) {
//         line = "";
//       }
//     }
//   }
// }