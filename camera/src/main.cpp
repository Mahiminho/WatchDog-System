#include <WiFi.h>
#include "esp_camera.h"
#define CAMERA_MODEL_AI_THINKER
#include "../Camera/camera_pins.h"
#include "../Camera/camera_index.h"
#include "HardwareSerial.h"

#define ESP32_CAM_UART_RX_PIN 2
#define ESP32_CAM_UART_TX_PIN 15
#define ESP32_CAM_UART_BAUD_RATE 115200

HardwareSerial extSerial(1);

void startCameraServer();
void setupLedFlash(int pin);

String ssid = "";
String password = "";

void waitForWiFiCredentials() {
  extSerial.println("Wait for WiFI credentials...");
  while (true) {
    if (extSerial.available()) {
      String input = extSerial.readStringUntil('\n');
      extSerial.println("ESP32-CAM recieved string: " + input);
      input.trim();
      int spaceIdx = input.indexOf(' ');
      if (spaceIdx > 0) {
        ssid = input.substring(0, spaceIdx);
        password = input.substring(spaceIdx + 1);
        extSerial.println("WiFi credentials recieved.");
        extSerial.flush();
        while (Serial.available()) {
          Serial.read();
        }

        break;
      }
      else {
        extSerial.println("Invalid format of WiFi credentials.");
      }
    }
    delay(100);
  }
}

void setup() {
  delay(30000); // give 30 sec for ESP-WROOM-32 to boot and start

  extSerial.begin(ESP32_CAM_UART_BAUD_RATE, SERIAL_8N1, ESP32_CAM_UART_RX_PIN, ESP32_CAM_UART_TX_PIN);
  extSerial.setDebugOutput(true);
  extSerial.println("ESP32-CAM stared.");
  waitForWiFiCredentials();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    }
    else {
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  }
  else {
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    extSerial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t* s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif

  // setup LED FLash if LED pin is defined in camera_pins.h
#if defined(LED_GPIO_NUM)
  setupLedFlash(LED_GPIO_NUM);
#endif

  // Serial.print("WiFi connecting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  extSerial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  extSerial.println("WiFi connected.");
  extSerial.print("Starting camera server...");
  startCameraServer();

  String url = "http://" + WiFi.localIP().toString();
  extSerial.println(url);
  delay(1000);
}

void loop() {
  static String url = "http://" + WiFi.localIP().toString();
  extSerial.println(url);
  delay(1000);
}