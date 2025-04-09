#include <Arduino.h>
#include <Thermometer.h>
#include <AirQuality.h>
#include <Light.h>

#define THERMOMETER_PIN 4
#define AIR_QUALITY_PIN 34
#define LIGHT_ADDRESS 0x23
#define LIGHT_SDA_PIN 21
#define LIGHT_SCL_PIN 22

Thermometer thermometer(THERMOMETER_PIN);
AirQuality airQuality(AIR_QUALITY_PIN);
Light light(LIGHT_ADDRESS, LIGHT_SDA_PIN, LIGHT_SCL_PIN);

void setup() {
  Serial.begin(9600);
  thermometer.begin();
  airQuality.begin();
  light.begin();
}

void loop() {
  float temperatureCelsius = thermometer.getTemperature();
  int airQualityPPM = airQuality.readPPM();
  float lightIntensityLux = light.readLux();

  delay(1000);
}