#include<SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <AccelStepper.h>
#include <ezButton.h>
#include <limits.h>

#define RAW_SPEED 50
#define RAW_MAX_SPEED 100
#define MICROSTEPS 16

#define VERTICAL_SWITCH_PIN 14 
#define HORIZONTAL_SWITCH_PIN 5
#define DEBOUNCE_TIME 50 // debounce time in milliseconds

#define INSTRUCTIONS_COUNT 13

#define ESP32_IP "192.168.42.35"

enum PickupState {
  NoPickUp        = 0,
  PickUpDown      = 1,
  PickUpForward   = 2,
  PickUpUp        = 3,
  PickUpBackward   = 4
};

enum Instruction {
  Stop            = 0,
  Forward         = 1, 
  Backward        = 2,
  TurnRight       = 3,
  TurnLeft        = 4,
  Right           = 5,
  Left            = 6,
  Up              = 7,
  Down            = 8,
  PickUp          = 9,
  Position        = 200
};

struct NamedInstruction {
  String name;
  Instruction instruction;
};

NamedInstruction instructions[INSTRUCTIONS_COUNT] = {
  { "stop", Stop },
  { "forward", Forward },
  { "backward", Backward }, 
  { "turnRight", TurnRight },
  { "turnLeft", TurnLeft },
  { "right", Right },
  { "left", Left },
  { "up", Up },
  { "down", Down }, 
  { "pickup", PickUp },
  { "position", Position }
};

// Replace with your network credentials
const char* ssid = "Lolo";
const char* password = "jesuispauvre";
const int stepPin = 12;
const int directionPin = 13;

const int speed = RAW_SPEED * MICROSTEPS;
const int maxSpeed = RAW_MAX_SPEED * MICROSTEPS;

ESP8266WebServer server;
AccelStepper verticalStepper = AccelStepper(AccelStepper::DRIVER, stepPin, directionPin);
ezButton verticalSwitch(VERTICAL_SWITCH_PIN);
ezButton horizontalSwitch(HORIZONTAL_SWITCH_PIN);

bool isGoingDown = false;
bool isDelivering = false;
bool verticalSwitchReleased = true;
bool horizontalSwitchReleased = true;

PickupState pickupState = NoPickUp;
unsigned long pickupForwardTimeout = ULONG_MAX;
unsigned long pickupBackwardTimeout = ULONG_MAX;
unsigned long pickupUpStop = ULONG_MAX;

void (*onDeliverFinished)() = NULL;
void (*onPickupFinished)() = NULL;

//Started SoftwareSerial at RX and GPIO02 pin of ESP8266/NodeMCU
SoftwareSerial s(4,2);

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  // Serial port to communicate with the Arduino
  s.begin(9600);

  // Connect to Wifi
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi..");
    delay(1000);
  }

  Serial.println("\nConnected.");
  Serial.print("IP Adress: ");
  Serial.println(WiFi.localIP());

  // Handle POST requests on /commands
  server.on("/commands", HTTP_POST, handleCommands);

  server.begin();

  verticalStepper.setMaxSpeed(maxSpeed);
  verticalSwitch.setDebounceTime(DEBOUNCE_TIME);
  horizontalSwitch.setDebounceTime(DEBOUNCE_TIME);
  startScan();
}

void loop() {

  // Vertical switch
  verticalSwitch.loop();

  if(verticalSwitch.isReleased()) { // for some reason isReleased is called when button is pressed and isPressed when it is released...
    Serial.println("VERT SWITCH PRESSED ");
    verticalSwitchReleased = false;
    if(pickupState == PickUpDown) {
      pickupForward();
    }
    if(isDelivering) {
      isDelivering = false;
      if(onDeliverFinished != NULL) {
        onDeliverFinished();
        onDeliverFinished = NULL;
      }
    }
  }
  if(verticalSwitch.isPressed()) {
      Serial.println("VERT SWICTH RELEASED ");
      verticalSwitchReleased = true;
  }

  // Horizontal switch
  horizontalSwitch.loop();
  if(horizontalSwitch.isReleased()) {
    Serial.println("HORIZ SWICTH PRESSED ");
    horizontalSwitchReleased = false;
    if(pickupState == PickUpForward) {
      pickupForwardTimeout = ULONG_MAX;
      pickupUp();
    }
  }
  if(horizontalSwitch.isPressed()) {
      Serial.println("HORIZ SWICTH RELEASED ");
      horizontalSwitchReleased = true; 
  }

  // Server
  if(pickupState == NoPickUp && !isDelivering) {
    server.handleClient();
  }

  // Vertical stepper
  if(!isGoingDown || (isGoingDown && verticalSwitchReleased))
      verticalStepper.runSpeed();

  // Pickup State
  if(pickupState == PickUpForward && millis() > pickupForwardTimeout) 
  {
    pickupForwardTimeout = ULONG_MAX;
    pickupState = NoPickUp;
    sendToArduino(Stop);
    setVerticalDirection(Stop);
  }
  else if(pickupState == PickUpUp && millis() > pickupUpStop) 
  {
    pickupUpStop = ULONG_MAX;
    setVerticalDirection(Stop);
    pickupBackward();
  } 
  else if(pickupState == PickUpBackward && millis() > pickupBackwardTimeout) 
  {
    setVerticalDirection(Stop);
    sendToArduino(Stop);
    if(onPickupFinished != NULL) {
      onPickupFinished();
      onPickupFinished = NULL;
    }
    pickupState = NoPickUp;
  }
}