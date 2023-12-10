#include <AccelStepper.h>
#include <SoftwareSerial.h>

#define RAW_SPEED 75
#define MICROSTEPS 16
#define NINETY_DEGREE_STEP 3625
#define ONE_UNIT_LENGTH 3300 ///time needed to activate the wheels from start to middle of first box position in the vertical axis

#define BACK_RIGHT_IDX 0
#define BACK_LEFT_IDX 1
#define FRONT_RIGHT_IDX 2
#define FRONT_LEFT_IDX 3



const int speed = RAW_SPEED * MICROSTEPS;
const int nbBoxes = 6;
const int enablePin = 8;
const int stepPins[4] = {2, 3, 4, 12};
const int directionPins[4] = {5, 6, 7, 13};


AccelStepper wheels[4] = {
    AccelStepper(AccelStepper::DRIVER, stepPins[BACK_RIGHT_IDX], directionPins[BACK_RIGHT_IDX]),   // Back Right Wheel
    AccelStepper(AccelStepper::DRIVER, stepPins[BACK_LEFT_IDX], directionPins[BACK_LEFT_IDX]),     // Back Left Wheel
    AccelStepper(AccelStepper::DRIVER, stepPins[FRONT_RIGHT_IDX], directionPins[FRONT_RIGHT_IDX]), // Front Right Wheel
    AccelStepper(AccelStepper::DRIVER, stepPins[FRONT_LEFT_IDX], directionPins[FRONT_LEFT_IDX])    // Front Left Wheel
};


void Forward()
{
  wheels[FRONT_RIGHT_IDX].setSpeed(-speed);
  wheels[FRONT_LEFT_IDX].setSpeed(-speed);
  wheels[BACK_RIGHT_IDX].setSpeed(-speed);
  wheels[BACK_LEFT_IDX].setSpeed(-speed);
}

void Backward()
{
  wheels[FRONT_RIGHT_IDX].setSpeed(speed);
  wheels[FRONT_LEFT_IDX].setSpeed(speed);
  wheels[BACK_RIGHT_IDX].setSpeed(speed);
  wheels[BACK_LEFT_IDX].setSpeed(speed);
}

void Left()
{
  wheels[FRONT_RIGHT_IDX].setSpeed(-speed);
  wheels[FRONT_LEFT_IDX].setSpeed(speed);
  wheels[BACK_RIGHT_IDX].setSpeed(speed);
  wheels[BACK_LEFT_IDX].setSpeed(-speed);
}

void Right()
{
  wheels[FRONT_RIGHT_IDX].setSpeed(speed);
  wheels[FRONT_LEFT_IDX].setSpeed(-speed);
  wheels[BACK_RIGHT_IDX].setSpeed(-speed);
  wheels[BACK_LEFT_IDX].setSpeed(speed);
}

void turnClockwise()
{
  wheels[FRONT_RIGHT_IDX].setSpeed(speed);
  wheels[FRONT_LEFT_IDX].setSpeed(-speed);
  wheels[BACK_RIGHT_IDX].setSpeed(speed);
  wheels[BACK_LEFT_IDX].setSpeed(-speed);
}

void turnCounterClockwise()
{
  wheels[FRONT_RIGHT_IDX].setSpeed(-speed);
  wheels[FRONT_LEFT_IDX].setSpeed(speed);
  wheels[BACK_RIGHT_IDX].setSpeed(-speed);
  wheels[BACK_LEFT_IDX].setSpeed(speed);
}




void setup() {

// Listen to instructions from esp
  Serial.begin(9600);

    pinMode(enablePin, OUTPUT);

    unsigned long horizontal_step = ONE_UNIT_LENGTH * 0.4;

    unsigned long vertical_step = ONE_UNIT_LENGTH; 
   
    unsigned long map[nbBoxes/2][2]; // 2D array 

// Fill the matrix with values
for (int i = 0; i < nbBoxes/2; i++) {
    double yPos = (i + 1) * vertical_step;
    map[i][0] = horizontal_step;  // x_pos for the first column of the first 2D matrix
    map[i][1] = yPos;             // y_pos for the first column of the first 2D matrix
}
   /*
    for (int i = 0; i < nbBoxes/2; i++) {
        Serial.print("(");
        Serial.print(map[i][0].x);
        Serial.print(", ");
        Serial.print(map[i][0].y);
        Serial.print(") - (");
        Serial.print(map[i][1].x);
        Serial.print(", ");
        Serial.print(map[i][1].y);
        Serial.println(")");
    }*/

}

void loop()
{
  int box_nb = Serial.read();
  if (box_nb == -1)
    return;
    
  digitalWrite(enablePin, LOW); // Connect motors

  goToPosition(box_nb);
}

void goToPosition(int box_nb)
{
  if(box_nb <= nbBoxes/2){
     pickUp(box_nb,0);
  }
     
  else {
      pickUp(box_nb % nbBoxes/2,1);
  }
}

void move(unsigned long step)
{
  unsigned long endTime = millis() + step;
  while (millis() < endTime)
  {
    for (int i = 0; i < 4; i++)
    {
      wheels[i].runSpeed();
    }
  }
}

void pickUp(int box_nb, int isOnRightSide)
{ 
  Forward();
  move(map[box_nb][1]);
  isOnRightSide == 1 ? turnClockwise() : turnCounterClockwise();
  move(NINETY_DEGREE_STEP);
  delay(500);
  Forward();
  move(map[box_nb][0]);
  delay(500);
  goBackToStart(box_nb,isOnRightSide);
}

void goBackToStart(int box_nb,int isOnRightSide)
{ 
    Backward();
    move(map[box_nb][0]);
    isOnRightSide == 1 ? turnCounterClockwise() : turnClockwise();
    move(NINETY_DEGREE_STEP);
    Forward();
    move(map[box_nb][1]);
}
