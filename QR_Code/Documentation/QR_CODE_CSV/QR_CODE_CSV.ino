#include <ESP32CameraPins.h>
#include <ESP32QRCodeReader.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <esp_camera.h>
#include "qrcode.h"
#include <strings.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// Wi-Fi credentials
const char* ssid = "Lolo";
const char* password = "jesuispauvre";

// Server configuration
const char* serverAddress = "http://192.168.42.122:3000/processQrCode"; // Replace with your computer's IP address




// Camera configuration
ESP32QRCodeReader reader(CAMERA_MODEL_AI_THINKER);
String scan1 = "No Scan";
String scan2 = "No Scan";
String scan3 = "No Scan";
char scan[] = "No Scan";
bool newQR = true;

void initCamera(){

}

// Task on what to do when a QR code is found
void onQrCodeTask(void *pvParameters)
{
  struct QRCodeData qrCodeData;
  Serial.print("onQRCodeData is called");
  Serial.print("there is a new qr :");
  Serial.print(newQR==1);

  while (true)
  { if (newQR){
      if (reader.receiveQrCode(&qrCodeData, 100))
      {
        Serial.println("Found QRCode");
        if (qrCodeData.valid)
        {
          newQR = false;
          String qrCodePayload = (const char *)qrCodeData.payload;
          Serial.print("Payload: ");
          Serial.println(qrCodePayload);  

          if (qrCodePayload != scan1)
          {
            const char* t_type = "miss";

            Serial.println("New QR Code");
            Serial.println(qrCodePayload);

            // Send the ID to the server
            sendIdToServer((const char *)qrCodeData.payload);
            scan3 = scan2;
            scan2 = scan1;
            scan1 = qrCodePayload;
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
    else{
      while (Serial.available() == 0) {
      }
      int input = Serial.parseInt();
      if(input==1){
        newQR = true;
      }
    }
  }
}


// Process the string
String processor(const String& var){
  if(var == "scan1"){
    return String(scan1);
  }
  else if(var == "scan2"){
    return String(scan2);
  }
  else if(var == "scan3"){
    return String(scan3);
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
  
    char requestUrl[100]; // Assuming a fixed size for the request URL

    strcpy(requestUrl, serverAddress);
    strcat(requestUrl, "?id=");
    strcat(requestUrl, id);

    printf("Request URL: %s\n", requestUrl);


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
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);

  connectToWiFi();
  initCamera();

  AsyncWebServer server(80);
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

