/*
 * Created by Inés Araujo
 * 
 * Track the position of the fork using an end switch and a stepper to distance conversion
 * 
 * Link to source code of ezButton: https://github.com/ArduinoGetStarted/button/blob/master/src/ezButton.cpp
 * 
 * Parameters to watch:
 * - Debounce time (setup)
 * - Speed
 * 
 */

#include <ezButton.h>
#include <AccelStepper.h>

#define END_SWITCH_PIN 7
#define DEBOUNCE_TIME 50 // debounce time in milliseconds

#define HOME 0 // Minimum distance from the floor (in steps)
// TODO: calibrate and find the value of TOP
#define TOP 3000 // Maximum distance from the floor (in steps)

#define stepPin 2
#define directionPin 5

#define SPEED 150

unsigned long x_steps;  // height in steps, HOME <= x <= TOP
ezButton limitSwitch(END_SWITCH_PIN);  // create ezButton object that attaches to pin 7;
AccelStepper axe = (AccelStepper::DRIVER, stepPin, directionPin);

void setup() {
  Serial.begin(9600);
  limitSwitch.setDebounceTime(DEBOUNCE_TIME);
  axe.setMaxSpeed(SPEED);
  // Go to HOME
  goDown(TOP);
}

void loop() {
  limitSwitch.loop(); // MUST call the loop() function first for the limit switch

  /* Zone for tests */

  /* End of tests */
}

void goUp(long dist){
  // check if we reached the TOP
  // go up
  // check again
  long count = dist;
  axe.setSpeed(SPEED);
  if(dist > 0 && x_steps < TOP){
    if(axe.runSpeed()){
      x_steps++;
      count--;}
  } else {
    x_steps = TOP;
  }
}

void goDown(long dist){
  // check that limit switch is untouched
  // go down
  // check again
  long count = dist;
  axe.setSpeed(-SPEED);
  if(dist > 0 && !limitSwitch.isPressed()){
    if(axe.runSpeed()){
      x_steps--;
      count--;
      }
  } else {
    x_steps = HOME;
  }
}

