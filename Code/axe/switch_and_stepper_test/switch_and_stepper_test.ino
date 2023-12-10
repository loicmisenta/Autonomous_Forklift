/*
* Code to control the forklift axe
*
* The stepper makes 200 pulses for making one full cycle rotation
* Test procedure: stepper moves two cycles up, then two down, and stops once the limit switch is pressed
*/

// define pin for the limit switch
#define limitSwitchPin D0

// define pins for the axe
#define stepPin D6
#define dirPin D7
#define stepDelay 700

#define stepsInCycle 200
const unsigned long maxSteps = 35 * stepsInCycle; // 40 is roughly the max number of steps we can make from top to bottom using the whole axe (never make this number of steps when the fork is in the structure)
unsigned long currentSteps = 0;
bool goingToHome = true; // Know if we are bringing the fork to home to avoid exceptions with currentSteps

void setup() {
  Serial.begin(9600);
  // Set the pin for the limit switch as Input
  pinMode(limitSwitchPin, INPUT);
  // Set the two pins for the axe as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  // Home the axe
  goingToHome = false;
  goDown(maxSteps);
  
  currentSteps = 0;
}

void loop() {
  // Put here the code to communicate with the main algorithm and awai for instructions

  /* Zone for tests */

  /* End of tests */
}

/*
* The stepper moves dist steps up or stops if the fork went too high
*
* long dist  Number of steps to make
* returns true if dist steps were made, false if stepper stopped before completing dist steps
*/
void goUp(long dist){
  long count = dist;
  digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
  while(count > 0){ 
    if(!tooHigh()){
      // Make one step and decrease counter
      if(oneStep()){ 
        --count;
        // Update position
        ++currentSteps;
      }
    } else {
      delay(1000);
      return false;
    }
  }
  delay(1000);
  return true;
}

/*
* The stepper moves dist steps down or stops if the limit switch is pressed
*
* long dist  Number of steps to make
* returns true if dist steps were made, false if stepper stopped before completing dist steps
*/
bool goDown(long dist){
  long count = dist;
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  while(count > 0){
    if(!switchIsPressed()){
      // Make one step and decrease counter
      if(oneStep()){
        --count;
        //  Update position
        if(!goingToHome){--currentSteps;}
      }
    } else {
       delay(1000);
       return false;
    }
  }
  delay(1000);
  return true;
}

/* Make one step 
**
* Always set step direction before calling this function
* Update currentSteps after calling this function
*/
bool oneStep(){
  digitalWrite(stepPin,HIGH); 
  delayMicroseconds(stepDelay);
  digitalWrite(stepPin,LOW); 
  delayMicroseconds(stepDelay);
  return true;
}

bool switchIsPressed(){
  return digitalRead(limitSwitchPin) == HIGH;
}

bool tooHigh(){
  return currentSteps >= maxSteps;
}

/* Call this function to measure the maximum number of revolutions the stepper can make to move the fork from the bottom of the axe to the top
*  Fork will go up to maxSteps at most
* This function is intended to be used to modify maxSteps
* IMPORTANT: press the limit switch at the bottom of the axe to stop the stepper from moving further. Will return number of revolutions completed
*/
int calibrate(){
  int steps = 0;
  bool up = true;
  while(!switchIsPressed() && up){
    up = goUp(stepsInCycle);
    ++steps;
    Serial.println(steps);
  }
  return steps;
}

/* Move the fork to a given position
*  long height  Height to move the fork to, in steps
*  return  true if fork as succesfully moved to the given position, false in case of expection
*/
bool goTo(long height){
  if(height < 0 || height > maxSteps){return false;}

  long dist = height - currentSteps;
  if(dist < 0){
    goDown(-dist);
  } else if(height - currentSteps > 0){
    goUp(dist);
  }
  return true;
}