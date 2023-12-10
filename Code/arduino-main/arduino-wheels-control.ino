void setDirection(Instruction direction) 
{
  switch(direction) {
    case Forward:
      forward();
      break;
    case Backward:
      backward();
      break;
    case Left:
      left();
      break;
    case Right:
      right();
      break;
    case TurnLeft:
      turnCounterClockwise();
      break;
    case TurnRight:
      turnClockwise();
      break;
    case Stop:
      stop();
      break;
    default:
      break;
  }

}

void move()
{
  for (int i = 0; i < 4; i++)
  {
    wheels[i].runSpeed();
  }
}

void forward()
{
  wheels[FRONT_RIGHT_IDX].setSpeed(-speed);
  wheels[FRONT_LEFT_IDX].setSpeed(-speed);
  wheels[BACK_RIGHT_IDX].setSpeed(-speed);
  wheels[BACK_LEFT_IDX].setSpeed(-speed);
}

void backward()
{
  wheels[FRONT_RIGHT_IDX].setSpeed(speed);
  wheels[FRONT_LEFT_IDX].setSpeed(speed);
  wheels[BACK_RIGHT_IDX].setSpeed(speed);
  wheels[BACK_LEFT_IDX].setSpeed(speed);
}

void left()
{
  wheels[FRONT_RIGHT_IDX].setSpeed(-speed);
  wheels[FRONT_LEFT_IDX].setSpeed(speed);
  wheels[BACK_RIGHT_IDX].setSpeed(speed);
  wheels[BACK_LEFT_IDX].setSpeed(-speed);
}

void right()
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

void diagonalFrontLeft()
{
  wheels[FRONT_RIGHT_IDX].setSpeed(speed);
  wheels[FRONT_LEFT_IDX].setSpeed(-speed);
  wheels[BACK_RIGHT_IDX].setSpeed(-speed);
  wheels[BACK_LEFT_IDX].setSpeed(speed);
}

void stop()
{
  brake();
  delay(500);
  enabled = false;
  digitalWrite(enablePin, HIGH); // Disconnect motors
}

void brake() 
{
  wheels[FRONT_RIGHT_IDX].setSpeed(0);
  wheels[FRONT_LEFT_IDX].setSpeed(0);
  wheels[BACK_RIGHT_IDX].setSpeed(0);
  wheels[BACK_LEFT_IDX].setSpeed(0);
}

void enable()
{
  enabled = true;
  digitalWrite(enablePin, LOW); // Connect motors
}