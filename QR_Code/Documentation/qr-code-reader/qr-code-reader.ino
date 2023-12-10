#include <Arduino.h>
#include <ESP32QRCodeReader.h>

#include <DNSServer.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include "ESPAsyncWebServer.h"

#include <ArduinoJson.h>


StaticJsonDocument<200> doc;

ESP32QRCodeReader reader(CAMERA_MODEL_AI_THINKER);

// Set these to your desired credentials.
const char *ssid = "Lolo";
const char *password = "jesuispauvre";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

String scan1 = "No Scan";
String scan2 = "No Scan";
String scan3 = "No Scan";
char scan[] = "No Scan";

bool newQR = false;


//annex functions

bool decodeJson(char json[]){
 

  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return false;
  }
  return true;
}

int getStorageCabinet(){
  return doc["StorageCabinet"];
}

int getFloor(){
  return doc["Floor"];
}
const char* getType(){
  return doc["Type"];
}
const char* getDescription(){
  return doc["Description"];
}


void encodeData(int storageCabinet, int floor, const char* type, char* BUFFER){
  sprintf(BUFFER, "%d %d %s", storageCabinet, floor, type);
}

void decodeData(int* storageCabinetAndFloor, char* type, char* BUFFER){
    char typeBuffer[32];
    int storageCabinet;
    int floor;
    sscanf(BUFFER, "%d %d %s", &storageCabinet, &floor, typeBuffer);
    storageCabinetAndFloor[0] = storageCabinet;
    storageCabinetAndFloor[1] = floor;
    strcpy(type, typeBuffer);
}


// Initialize WiFi (STATIC IP) -- not used, don't work...
void initWiFi() {

    WiFi.softAP("Lolo", password);

    delay(2000);
    // Set static IP
    IPAddress AP_LOCAL_IP(192, 168, 1, 1);
    IPAddress AP_GATEWAY_IP(192, 168, 1, 4);
    IPAddress AP_NETWORK_MASK(255, 255, 255, 0);
    if (!WiFi.softAPConfig(AP_LOCAL_IP, AP_GATEWAY_IP, AP_NETWORK_MASK)) {
        Serial.println("AP Config Failed");
        return;
    }
    Serial.println("WiFi AP setup");
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());
    
}


void initWiFi2(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println(WiFi.localIP());
}


String processor(const String& var){
  //getSensorReadings();
  //Serial.println(var);
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


const char index_html[] PROGMEM = R"rawliteral(
</html><!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p { font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-image: linear-gradient(to bottom right, rgba(50, 192, 243, 0.5),rgba(27, 55, 131, 0.5), rgba(2, 251, 255, 0.6)); color: white; font-size: 1rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 800px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(400px, 1fr)); }
    .reading { font-size: 1.4rem; }
  </style>
</head>
<body>
  <div class="topnav">
    <h1>ForkLift Project</h1>
    <h1>QR Code & Data Read</h1>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card">
        <p>Storage Cabinet</p><p><span class="reading"><span id="storageCabinet">###</span></p>
      </div>
      <div class="card">
        <p>Floor</p><p><span class="reading"><span id="floor">###</span></p>
      </div>
      <div class="card">
        <p>Type</p><p><span class="reading"><span id="type">###</span></p>
      </div>
    </div>
  </div>
  <br>
  <br>
  <br>
<script>
if (!!window.EventSource) {
 var source = new EventSource('/events');
 
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 
 source.addEventListener('storageCabinet', function(e) {
  console.log("storageCabinet", e.data);
  document.getElementById("storageCabinet").innerHTML = e.data;
 }, false);
 
 source.addEventListener('floor', function(e) {
  console.log("floor", e.data);
  document.getElementById("floor").innerHTML = e.data;
 }, false);
 
 source.addEventListener('type', function(e) {
  console.log("type", e.data);
  document.getElementById("type").innerHTML = e.data;
 }, false);
}
</script>
</body>
</html>)rawliteral";


void onQrCodeTask(void *pvParameters)
{
  struct QRCodeData qrCodeData;

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

            int storageCabinetAndFloor[2];
            char type[64];
            //char BUFFER[64];
            decodeData(storageCabinetAndFloor, type, (char *)qrCodeData.payload);
            int storageCabinet = storageCabinetAndFloor[0];
            int floor = storageCabinetAndFloor[1];


            //MAKE OWN DECODE/ENCODE METHOD
            //bool asDecode = decodeJson(scan);
            //if (asDecode) {
            //  t_type = getType();
            //}

            Serial.print(type);
            Serial.print(storageCabinet);
            Serial.print(floor);

            // Send Events to the Web Server with the Sensor Readings
            events.send("ping",NULL,millis());
            events.send(String(type).c_str(),"type",millis());
            events.send(String(storageCabinet).c_str(),"storageCabinet",millis());
            events.send(String(floor).c_str(),"floor",millis());


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

void setup()
{
  newQR = true;
  Serial.begin(115200);
  Serial.println();

  //your other setup stuff...
  
  // Init WiFi
  initWiFi2();
  // Handle Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  server.begin();


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



