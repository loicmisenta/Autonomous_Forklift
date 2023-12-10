#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <esp_camera.h>
#include "qrcode.h"

// Wi-Fi credentials
const char* ssid = "Lolo";
const char* password = "jesuispauvre";

// Server configuration
const char* serverAddress = "http://128.179.130.222:3000/processQrCode"; // Replace with your computer's IP address

// Camera configuration
const int cameraId = 0; // ID of the camera to be used (0 for the default camera)

// QR code scanning variables
QRCode qrcode;
bool qrCodeDetected = false;
String qrCodeData;

// Initialize the camera
void initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Initialize the camera module
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera initialization failed with error 0x%x", err);
    while (1);
  }
}

// Callback function for QR code detection
void qrCodeCallback(uint8_t* buf, size_t len) {
  qrCodeData = "";
  for (size_t i = 0; i < len; i++) {
    qrCodeData += (char)buf[i];
  }
  qrCodeDetected = true;
}

// Connect to Wi-Fi network
void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
}

// Send ID number to the server via HTTP
void sendIdToServer(String id) {
  HTTPClient http;
  
  String requestUrl = serverAddress + "?id=" + id;

  http.begin(requestUrl);
  int httpResponseCode = http.GET();
  
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Server response: " + response);
    // Process the response and take appropriate action (e.g., turn on the LED)
  } else {
    Serial.println("Error sending request to the server");
  }
  
  http.end();
}

void setup() {
  Serial.begin(115200);

  connectToWiFi();
  initCamera();

  camera_config_t* config = esp_camera_get_config();
  config->sensor.reset_pin = 32;
  config->sensor.sda_pin = 26;
  config->sensor.scl_pin = 27;
  config->sensor.size.width = 800;
  config->sensor.size.height = 600;
  config->sensor.pixformat = PIXFORMAT_GRAYSCALE;

  esp_camera_set_config(0, config);

  qrcode.setCallback(qrCodeCallback);

  // Start the web server
  AsyncWebServer server(80);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  if (qrCodeDetected) {
    Serial.println("QR Code Detected: " + qrCodeData);
    sendIdToServer(qrCodeData);
    qrCodeDetected = false;
  }

  camera_fb_t* fb = esp_camera_fb_get();

  if (fb) {
    qrcode.scan(fb->buf, fb->len);
    esp_camera_fb_return(fb);
  }
}

