void setVerticalDirection(Instruction instruction) {
  int actual_speed = 0;

  switch(instruction) {
    case Up:
      Serial.println("Going up");
      sendToArduino(Stop);
      actual_speed = -speed;
      isGoingDown = false;
      break;
    case Down:
      Serial.println("Going down");
      sendToArduino(Stop);
      actual_speed = speed;
      isGoingDown = true;
      break;
    case Stop:
      Serial.println("Stop");
      isGoingDown = false;
      break;
    default:
      break;
  }

  verticalStepper.setSpeed(actual_speed);
}