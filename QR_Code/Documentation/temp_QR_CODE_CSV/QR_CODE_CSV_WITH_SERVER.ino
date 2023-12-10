#include <WiFi.h>
#include <HTTPClient.h>
#include <ESPAsyncWebServer.h>
#include <esp_camera.h>
#include "qrcode.h"

// Wi-Fi credentials
const char* ssid = "Lolo";
const char* password = "jesuispauvre";

// Server configuration
const char* serverAddress = "http://192.168.42.122:3000/processQrCode"; // Replace with your computer's IP address

// Camera configuration
ESP32QRCodeReader reader(CAMERA_MODEL_AI_THINKER);
String scan = "No Scan";
bool newQR = true;
bool send_success = false;

// Task on what to do when a QR code is found
void onQrCodeTask(void *pvParameters)
{
  struct QRCodeData qrCodeData;
  
  while (true)
  { 
    if (newQR)
    {
      if (reader.receiveQrCode(&qrCodeData, 100))
      {
        Serial.println("Found QRCode");
        if (qrCodeData.valid)
        {
          newQR = false;
          String qrCodePayload = (const char *)qrCodeData.payload;
          Serial.print("Payload: ");
          Serial.println(qrCodePayload);  

          if (qrCodePayload != scan)
          {
            Serial.println("New QR Code");
            Serial.println(qrCodePayload);

            // Send the ID to the server
            sendIdToServer(qrCodePayload.c_str());
            scan = qrCodePayload;
          }
        }
        else
        {
          Serial.print("Invalid: ");
          Serial.println((const char *)qrCodeData.payload);
        }
      }
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    else
    {
      while (Serial.available() == 0) {
      }
      int input = Serial.parseInt();
      if (input == 1){
        newQR = true;
      }
    }
  }
}

// Process the string
String processor(const String& var){
  if(var == "scan"){
    return String(scan);
  }
  return String();
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
void sendIdToServer(const char* id) {
  HTTPClient http;
  String requestUrl = serverAddress;
  requestUrl += "?id=";
  requestUrl += id;

  Serial.print("Request URL: ");
  Serial.println(requestUrl);

  http.begin(requestUrl);
  int httpResponseCode = http.GET();
  
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Server response: " + response);
    send_success = true;
    // Process the response and take appropriate action (e.g., turn on the LED)
  } else {
    Serial.println("Error sending request to the server");
    send_success = false;
  }
  
  http.end();
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);

  connectToWiFi();

  AsyncWebServer server(80);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Hello, World!");
  });

  server.begin();
  Serial.println("Web server started");

  reader.setup();

  Serial.println("Setup QRCode Reader");

  reader.beginOnCore(1);

  Serial.println("Begin on Core 1");

  xTaskCreate(onQrCodeTask, "onQrCode", 4 * 1024, NULL, 4, NULL);
}

void loop()
{
  delay(100);
}
