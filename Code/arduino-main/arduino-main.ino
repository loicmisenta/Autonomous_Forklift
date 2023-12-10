#include <AccelStepper.h>
#include<SoftwareSerial.h>

#define RAW_SPEED 75
#define RAW_MAX_SPEED 300
#define MICROSTEPS 16

#define BACK_LEFT_IDX 0
#define FRONT_LEFT_IDX 1
#define BACK_RIGHT_IDX 2
#define FRONT_RIGHT_IDX 3

#define ONE_UNIT_LENGTH 7500
#define NINETY_DEGREE_STEP 3500

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
};

const int speed = RAW_SPEED * MICROSTEPS;
const int maxSpeed = RAW_MAX_SPEED * MICROSTEPS;

const int enablePin = 8;
const int stepPins[4] = {2, 3, 4, 12};
const int directionPins[4] = {5, 6, 7, 13};

AccelStepper wheels[4] = {
    AccelStepper(AccelStepper::DRIVER, stepPins[BACK_LEFT_IDX], directionPins[BACK_LEFT_IDX]),   // Back Right Wheel
    AccelStepper(AccelStepper::DRIVER, stepPins[FRONT_LEFT_IDX], directionPins[FRONT_LEFT_IDX]),     // Back Left Wheel
    AccelStepper(AccelStepper::DRIVER, stepPins[BACK_RIGHT_IDX], directionPins[BACK_RIGHT_IDX]), // Front Right Wheel
    AccelStepper(AccelStepper::DRIVER, stepPins[FRONT_RIGHT_IDX], directionPins[FRONT_RIGHT_IDX])    // Front Left Wheel
};

bool enabled;

void setup()
{
  // Listen to instructions from esp
  Serial.begin(9600);

  pinMode(enablePin, OUTPUT);
  stop();
  for (int i = 0; i < 4; i++)
  {
    wheels[i].setMaxSpeed(maxSpeed);
  }
}

void loop()
{
  handleInstructions();
  move();
}