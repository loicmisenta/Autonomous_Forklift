void handleCommands() {
  String plain = server.arg("plain");
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, plain);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  for(int i = 0; i < INSTRUCTIONS_COUNT; i++) {
    NamedInstruction namedInstr = instructions[i];

    JsonVariant jsonVar = doc[namedInstr.name];
    if(!jsonVar.isNull()) {
      dispatchInstruction(namedInstr.instruction, jsonVar.as<String>());
    }
  }

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "Hello man");
}

void startScan() {
  WiFiClient client;
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(client, "http://192.168.42.35/start-scan");

    // If you need Node-RED/server authentication, insert user and password below
    //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

    // Specify content-type header
    http.addHeader("Content-Type", "text/plain");   
    // Send HTTP POST request
    int httpResponseCode = http.POST("");

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
        
    // Free resources
    http.end();
}