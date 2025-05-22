#include "SensorsData.h"

SensorsData::SensorsData(uint8_t thermometer_pin,
                         uint8_t air_quality_pin,
                         uint8_t light_address,
                         uint8_t light_sda_pin,
                         uint8_t light_scl_pin,
                         uint8_t human_presence_rx_pin,
                          uint8_t human_presence_tx_pin,
                         uint8_t motion_detection_pin,
                         uint8_t audio_pin,
                         uint8_t vibration_detection_pin)

  : Thermometer(thermometer_pin),
  AirQuality(air_quality_pin),
  Light(light_address, light_sda_pin, light_scl_pin),
  HumanPresence(human_presence_rx_pin, human_presence_tx_pin),
  MoveDetection(motion_detection_pin),
  Audio(audio_pin),
  VibrationDetection(vibration_detection_pin) {
}

void SensorsData::initSensors() {
  Thermometer::begin();
  AirQuality::begin();
  Light::begin();
  HumanPresence::begin();
  MoveDetection::begin();
  Audio::begin();
  VibrationDetection::begin();
}

void SensorsData::collectData() {
  data.temperatureCelsius = Thermometer::getTemperature();
  data.airQualityPPM = AirQuality::readPPM();
  data.lightIntensityLux = Light::readLux();
  data.presenceDetected = HumanPresence::isPresent();
  data.presenceDistance = HumanPresence::distance();
  data.motionDetected = MoveDetection::isMotionDetected();
  data.noiseLevelDB = Audio::getNoiseLevel();
  data.vibrationDetected = VibrationDetection::isVibrationDetected();
}

SensorReadings SensorsData::getData() const {
  return data;
}