#include <ezButton.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

#include <ESP32CameraPins.h>
#include <ESP32QRCodeReader.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <ESPAsyncWebServer.h>
#include <esp_camera.h>
#include "qrcode.h"

#include <ArduinoJson.h>
#include <strings.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"


// -------------------------------------------------------//
// ------------- WIFI / SERVER CONFIG ------------------- //
// -------------------------------------------------------//

// Wi-Fi credentials
const char* ssid = "Lolo";
const char* password = "jesuispauvre";

// Server configuration
const int serverPort = 80;

// Create an instance of the web server
AsyncWebServer server(serverPort);

// Server configuration
const char* serverAddress = "http://192.168.42.122:3000/processQrCode"; // Replace with your computer's IP address


// -------------------------------------------------------//
// ---------------- CAMERA CONFIG ----------------------- //
// -------------------------------------------------------//

// Camera configuration
ESP32QRCodeReader reader(CAMERA_MODEL_AI_THINKER);


// -------------------------------------------------------//
// ------------------ LED CONFIG ------------------------ //
// -------------------------------------------------------//

#define LED_BUILTIN 4

// Initialize LED
void initFlash() { 
  pinMode(LED_BUILTIN, OUTPUT);
}

// Turn on LED
void turnOnFlash() {
  digitalWrite(LED_BUILTIN, HIGH);  // Set LEDC duty cycle to maximum (255)
}

// Turn off LED
void turnOffFlash() {
  digitalWrite(LED_BUILTIN, LOW);  // Set LEDC duty cycle to 0 (off)
}


// -------------------------------------------------------//
// ----------- SERVER SCAN REQUEST CONFIG --------------- //
// -------------------------------------------------------//

// Flag to indicate if a scan is in progress
bool scanning = false;
bool newQR = true;
bool send_success = false;

// Route handler for the POST request to start a scan
void handleStartScan(AsyncWebServerRequest* request) {
  if (request->method() == HTTP_POST) {
    // Check if a scan is already in progress
    if (scanning) {
      request->send(200, "text/plain", "Scan already in progress");
      Serial.println("Scan already in progress");
    } 

    else {
      // Start the scan
      request->send(200, "text/plain", "Scan started");
      Serial.println("Scan started");
      scanning = true;
      newQR = true;
    }
  }
}


// -------------------------------------------------------//
// ----------- SCAN QR CODE CONFIG / HANDLE ------------- //
// -------------------------------------------------------//

String scan = "No Scan";

// Task on what to do when a QR code is found
void onQrCodeTask(void *pvParameters)
{
  
  Serial.print("onQRCodeData is called");
  struct QRCodeData qrCodeData;
  Serial.print("there is a new qr :");
  Serial.print(newQR==1);
  
  while (true)
  { 
    delay(200);
    Serial.print(".");
    if(scanning){
      //qrCodeData = {false, 0, {0}, 0};
      //memset(payload, 0, sizeof(payload));
      if (newQR)
      {
        if (reader.receiveQrCode(&qrCodeData, 100))
        {
          // ESP32 find QR Code
          Serial.println("Found QRCode");

          // Check if QR code is valid 
          if (qrCodeData.valid)
          {


            // String that is on the qr code
            String qrCodePayload = (const char *)qrCodeData.payload;

            Serial.print("scan : ");
            Serial.println(qrCodePayload);  

            // Check if this is a new qr code
            if (qrCodePayload != scan)
            {
              newQR = false;
              Serial.println("this was a new QR Code");

              // Send the ID to the server until success 
              while(!send_success){
                sendIdToServer(qrCodePayload.c_str());
                Serial.println("Sending ID to server...");

                if (send_success){
                  Serial.println("ID sent to server");

                  //do a flash to confirm that id was sent to server
                  turnOnFlash();
                  delay(400);
                  turnOffFlash();
                  delay(400);
                  turnOnFlash();
                  delay(100);
                  turnOffFlash();

                  newQR = true;
                  scanning = false;
                }
                else{
                  Serial.println("Error sending ID to server");
                  Serial.println("Retrying...");
                }
              }
              scan = qrCodePayload;
              send_success = false;
              //qrCodeData = {false, 0, {0}, 0};
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
}


// -------------------------------------------------------//
// ------------- WIFI CONNECTION CONFIG ----------------- //
// -------------------------------------------------------//

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


// -------------------------------------------------------//
// --------- HANDLE SENDING ID TO SERVER ---------------- //
// -------------------------------------------------------//

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
    // Process the response and take appropriate action
  } else {
    Serial.println("Error sending request to the server");
    send_success = false;
  }
  Serial.print("end of sendIdToServer");
  
  http.end();
}


// -------------------------------------------------------//
// --------------- MAIN CONFIGURATION ------------------- //
// -------------------------------------------------------//

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);

  initFlash();
  turnOffFlash();

  connectToWiFi();

  // Route handler for the POST request to start a scan
  server.on("/start-scan", HTTP_POST, handleStartScan);

  // Start the server
  server.begin();
  Serial.println("Web server started");
  Serial.println(WiFi.localIP());

  reader.setup();

  Serial.println("Setup QRCode Reader");

  reader.beginOnCore(1);

  Serial.println("Begin on Core 1");

  turnOnFlash();
  delay(100);
  turnOffFlash();

  xTaskCreate(onQrCodeTask, "onQrCode", 4 * 1024, NULL, 4, NULL);
}

// -------------------------------------------------------//
// ------------------------ LOOP ------------------------ //
// -------------------------------------------------------//

void loop()
{
  delay(100);
}
