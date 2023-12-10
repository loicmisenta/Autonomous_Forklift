/*
* Code to control the forklift axe
*
* The stepper makes 200 pulses for making one full cycle rotation
*/
// define pins for the axe
#define stepPin D6
#define dirPin D7
#define stepDelay 700
#define stepsInCycle 200

void setup() {
  Serial.begin(9600);
  // Set the two pins for the axe as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
}

void loop() {
  /* Zone for tests */
  goUp(200);
  delay(1000);
  goDown(200);
  delay(1000);
  /* End of tests */
}

void goUp(long dist){
  long count = dist;
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  while(count > 0){ //TODO: Infinite loop: add constraints on position
    // Make one step and decrease counter
    if(oneStep()){ --count;}
  }
}

void goDown(long dist){
  long count = dist;
  digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
  while(count > 0){ //TODO: Infinite loop: add constraints on position
    // Make one step and decrease counter
    if(oneStep()){ --count;}
  }
}

bool oneStep(){
  digitalWrite(stepPin,HIGH); 
  delayMicroseconds(stepDelay);
  digitalWrite(stepPin,LOW); 
  delayMicroseconds(stepDelay);
  return true;
}
